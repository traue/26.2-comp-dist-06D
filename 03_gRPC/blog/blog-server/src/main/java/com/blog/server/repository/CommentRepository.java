package com.blog.server.repository;

import com.blog.grpc.generated.Comment;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Sorts;
import org.bson.Document;
import org.bson.types.ObjectId;

import java.time.Instant;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

/**
 * Repositório de Comentários: acesso a dados para a coleção "comments".
 *
 * Relacionamento com Posts:
 *   - Cada comentário armazena o "postId" (string do ObjectId do post pai).
 *   - Esta é uma relação de referência (não embedding), pois comentários
 *     podem crescer indefinidamente e precisam ser consultados separadamente.
 *
 * No MongoDB, não há foreign key — a integridade referencial é garantida
 * pela aplicação (o serviço verifica se o post existe antes de comentar).
 */
public class CommentRepository {

    private static final Logger logger = Logger.getLogger(CommentRepository.class.getName());

    private final MongoCollection<Document> collection;

    public CommentRepository(MongoCollection<Document> collection) {
        this.collection = collection;
    }

    // =========================================================
    // CREATE
    // =========================================================

    /**
     * Persiste um novo comentário associado a um post.
     *
     * @param postId  ID do post pai (validado previamente pelo serviço)
     * @param author  Autor do comentário
     * @param content Texto do comentário
     * @return Comment criado com ID gerado pelo MongoDB
     */
    public Comment create(String postId, String author, String content) {
        String now = Instant.now().toString();

        Document doc = new Document()
                .append("postId",    postId)    // referência ao post pai
                .append("author",    author)
                .append("content",   content)
                .append("createdAt", now);

        collection.insertOne(doc);

        logger.info("Comentário criado. ID: " + doc.getObjectId("_id").toHexString()
                + " — Post: " + postId);
        return documentToComment(doc);
    }

    // =========================================================
    // READ
    // =========================================================

    /**
     * Busca todos os comentários de um post, ordenados por data de criação.
     *
     * Utiliza índice no campo "postId" para performance.
     * (O índice deve ser criado na inicialização do servidor — ver BlogServer)
     *
     * @param postId ID do post cujos comentários serão buscados
     * @return lista de comentários em ordem cronológica
     */
    public List<Comment> findByPostId(String postId) {
        List<Comment> comments = new ArrayList<>();

        try (MongoCursor<Document> cursor = collection.find(
                Filters.eq("postId", postId))   // filtra pelo campo "postId"
                .sort(Sorts.ascending("createdAt")) // mais antigos primeiro
                .cursor()) {

            while (cursor.hasNext()) {
                comments.add(documentToComment(cursor.next()));
            }
        }

        logger.info("Comentários do post " + postId + ": " + comments.size());
        return comments;
    }

    // =========================================================
    // DELETE
    // =========================================================

    /**
     * Remove um comentário pelo ID.
     *
     * @return true se encontrado e removido, false caso contrário
     */
    public boolean delete(String id) {
        try {
            ObjectId objectId = new ObjectId(id);
            var result = collection.deleteOne(Filters.eq("_id", objectId));
            return result.getDeletedCount() > 0;
        } catch (IllegalArgumentException e) {
            logger.warning("ID de comentário inválido: " + id);
            return false;
        }
    }

    /**
     * Busca um comentário por ID (necessário para descobrir o postId
     * antes de decrementar o contador no post pai).
     *
     * @return Document bruto ou null se não encontrado
     */
    public Document findRawById(String id) {
        try {
            ObjectId objectId = new ObjectId(id);
            return collection.find(Filters.eq("_id", objectId)).first();
        } catch (IllegalArgumentException e) {
            return null;
        }
    }

    // =========================================================
    // CONVERSÃO: Documento BSON → Mensagem Protobuf
    // =========================================================

    private Comment documentToComment(Document doc) {
        return Comment.newBuilder()
                .setId(doc.getObjectId("_id").toHexString())
                .setPostId(doc.getString("postId"))
                .setAuthor(doc.getString("author"))
                .setContent(doc.getString("content"))
                .setCreatedAt(doc.getString("createdAt"))
                .build();
    }
}
