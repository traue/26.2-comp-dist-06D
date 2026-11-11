package com.blog.server.repository;

import com.blog.grpc.generated.Post;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Sorts;
import com.mongodb.client.model.Updates;
import org.bson.Document;
import org.bson.types.ObjectId;

import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.logging.Logger;

/**
 * Repositório de Posts: camada de acesso a dados para a coleção "posts" no MongoDB.
 *
 * Responsabilidades:
 *   - Traduzir operações de negócio (criar, buscar, atualizar, deletar) em
 *     operações MongoDB usando o driver Java oficial.
 *   - Converter documentos BSON (formato nativo do MongoDB) para mensagens
 *     Protobuf (Post) que serão enviadas via gRPC.
 *
 * Padrão de Projeto: Repository — isola a lógica de persistência da lógica
 * de negócio (PostServiceImpl), facilitando testes e troca de banco de dados.
 */
public class PostRepository {

    private static final Logger logger = Logger.getLogger(PostRepository.class.getName());

    // Tamanho padrão de página quando o cliente não especifica
    private static final int DEFAULT_PAGE_SIZE = 10;

    // Coleção MongoDB — equivalente a uma "tabela" em bancos relacionais
    private final MongoCollection<Document> collection;

    public PostRepository(MongoCollection<Document> collection) {
        this.collection = collection;
    }

    // =========================================================
    // CREATE
    // =========================================================

    /**
     * Persiste uma nova postagem no MongoDB.
     *
     * O MongoDB gera automaticamente um ObjectId para o campo "_id".
     * Após o insert, o documento é atualizado com o ID gerado.
     *
     * @return Post com todos os campos preenchidos, incluindo o ID gerado
     */
    public Post create(String title, String content, String author) {
        String now = Instant.now().toString(); // ISO-8601 ex: "2024-01-15T10:30:00Z"

        Document doc = new Document()
                .append("title",        title)
                .append("content",      content)
                .append("author",       author)
                .append("createdAt",    now)
                .append("updatedAt",    now)
                .append("commentCount", 0);    // contador desnormalizado

        // insertOne modifica o documento in-place, adicionando _id
        collection.insertOne(doc);

        logger.info("Post criado. ID: " + doc.getObjectId("_id").toHexString());
        return documentToPost(doc);
    }

    // =========================================================
    // READ
    // =========================================================

    /**
     * Busca um post pelo ID (string hexadecimal do ObjectId MongoDB).
     *
     * @return Optional.empty() se o ID for inválido ou o post não existir
     */
    public Optional<Post> findById(String id) {
        try {
            ObjectId objectId = new ObjectId(id); // lança IllegalArgumentException se inválido
            Document doc = collection.find(Filters.eq("_id", objectId)).first();
            return Optional.ofNullable(doc).map(this::documentToPost);
        } catch (IllegalArgumentException e) {
            // ID com formato inválido — retorna vazio em vez de propagar exceção
            logger.warning("ID inválido recebido: " + id);
            return Optional.empty();
        }
    }

    /**
     * Busca posts paginados, ordenados do mais recente ao mais antigo.
     *
     * Paginação no MongoDB: skip() + limit()
     *   - skip(page * pageSize) → pula os itens das páginas anteriores
     *   - limit(pageSize)       → retorna no máximo pageSize itens
     *
     * @param page     número da página (começa em 0)
     * @param pageSize itens por página (0 usa DEFAULT_PAGE_SIZE)
     * @return lista de posts da página solicitada
     */
    public List<Post> findAll(int page, int pageSize) {
        if (pageSize <= 0) pageSize = DEFAULT_PAGE_SIZE;

        int skip = page * pageSize;
        List<Post> posts = new ArrayList<>();

        // try-with-resources fecha o cursor automaticamente após uso
        try (MongoCursor<Document> cursor = collection.find()
                .sort(Sorts.descending("createdAt")) // mais recentes primeiro
                .skip(skip)
                .limit(pageSize)
                .cursor()) {

            while (cursor.hasNext()) {
                posts.add(documentToPost(cursor.next()));
            }
        }

        logger.info(String.format("Listando posts — página: %d, tamanho: %d, retornados: %d",
                page, pageSize, posts.size()));
        return posts;
    }

    // =========================================================
    // UPDATE
    // =========================================================

    /**
     * Atualiza título e conteúdo de um post existente.
     *
     * Updates.combine() permite múltiplas atualizações em uma única
     * operação atômica no MongoDB (importante para consistência).
     *
     * @return Optional com post atualizado, ou vazio se não encontrado
     */
    public Optional<Post> update(String id, String title, String content) {
        try {
            ObjectId objectId = new ObjectId(id);
            String now = Instant.now().toString();

            collection.updateOne(
                    Filters.eq("_id", objectId),
                    Updates.combine(
                            Updates.set("title",     title),
                            Updates.set("content",   content),
                            Updates.set("updatedAt", now)
                    )
            );

            return findById(id); // rebusca para retornar o documento atualizado
        } catch (IllegalArgumentException e) {
            return Optional.empty();
        }
    }

    // =========================================================
    // DELETE
    // =========================================================

    /**
     * Remove um post da coleção.
     *
     * @return true se o documento foi encontrado e removido
     */
    public boolean delete(String id) {
        try {
            ObjectId objectId = new ObjectId(id);
            var result = collection.deleteOne(Filters.eq("_id", objectId));
            return result.getDeletedCount() > 0;
        } catch (IllegalArgumentException e) {
            return false;
        }
    }

    // =========================================================
    // HELPERS INTERNOS — atualização do contador de comentários
    // =========================================================

    /**
     * Incrementa atomicamente o contador de comentários.
     * Chamado pelo CommentServiceImpl ao adicionar um comentário.
     * Updates.inc() é atômico no MongoDB — seguro em ambiente concorrente.
     */
    public void incrementCommentCount(String postId) {
        updateCommentCount(postId, 1);
    }

    /** Decrementa atomicamente o contador de comentários. */
    public void decrementCommentCount(String postId) {
        updateCommentCount(postId, -1);
    }

    private void updateCommentCount(String postId, int delta) {
        try {
            ObjectId objectId = new ObjectId(postId);
            collection.updateOne(
                    Filters.eq("_id", objectId),
                    Updates.inc("commentCount", delta)
            );
        } catch (IllegalArgumentException ignored) {
            // postId inválido — não propaga, o CommentService já validou o post
        }
    }

    // =========================================================
    // CONVERSÃO: Documento BSON → Mensagem Protobuf
    // =========================================================

    /**
     * Converte um Document MongoDB para a mensagem Protobuf Post.
     *
     * Esta conversão é necessária porque o MongoDB armazena dados como BSON
     * e o gRPC transmite dados como Protocol Buffers — formatos diferentes.
     * O método centraliza o mapeamento de campos, evitando duplicação.
     */
    private Post documentToPost(Document doc) {
        return Post.newBuilder()
                .setId(doc.getObjectId("_id").toHexString())
                .setTitle(doc.getString("title"))
                .setContent(doc.getString("content"))
                .setAuthor(doc.getString("author"))
                .setCreatedAt(doc.getString("createdAt"))
                .setUpdatedAt(doc.getString("updatedAt"))
                .setCommentCount(doc.getInteger("commentCount", 0))
                .build();
    }
}
