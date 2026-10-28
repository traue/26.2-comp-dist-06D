// =============================================================
// Script de inicialização do MongoDB
// Executado automaticamente na PRIMEIRA vez que o container sobe
//
// Cria:
//   - Banco "blogdb"
//   - Usuário de aplicação com permissões limitadas
//   - Índices nas coleções
//   - Dados de exemplo para demonstração
// =============================================================

// Seleciona (ou cria) o banco da aplicação
db = db.getSiblingDB('blogdb');

// Cria um usuário com permissões apenas no banco da aplicação
// (princípio do menor privilégio — não use root na aplicação)
db.createUser({
    user: 'bloguser',
    pwd: 'blogpass',
    roles: [{ role: 'readWrite', db: 'blogdb' }]
});

// Cria as coleções explicitamente (opcional — MongoDB cria automaticamente)
db.createCollection('posts');
db.createCollection('comments');

// Cria índices para otimizar consultas
db.posts.createIndex({ createdAt: -1 }, { name: 'idx_posts_createdAt' });
db.comments.createIndex({ postId: 1 },  { name: 'idx_comments_postId' });

// -----------------------------------------------------------
// Dados de exemplo para demonstração em aula
// -----------------------------------------------------------

const post1Id = new ObjectId();
const post2Id = new ObjectId();

const agora = new Date().toISOString();

db.posts.insertMany([
    {
        _id: post1Id,
        title: 'Introdução ao gRPC',
        content: 'O gRPC é um framework de RPC de alta performance desenvolvido pelo Google. ' +
                 'Utiliza Protocol Buffers como formato de serialização e HTTP/2 como transporte, ' +
                 'oferecendo streaming bidirecional e eficiência na transmissão de dados.',
        author: 'Prof. Silva',
        createdAt: agora,
        updatedAt: agora,
        commentCount: 2
    },
    {
        _id: post2Id,
        title: 'Protocol Buffers vs JSON',
        content: 'Protocol Buffers (protobuf) é um formato de serialização binário, ' +
                 'tipicamente 3-10x menor e 20-100x mais rápido que JSON equivalente. ' +
                 'A desvantagem é não ser legível por humanos sem ferramentas específicas.',
        author: 'Prof. Costa',
        createdAt: agora,
        updatedAt: agora,
        commentCount: 1
    }
]);

db.comments.insertMany([
    {
        postId: post1Id.toString(),
        author: 'Aluno João',
        content: 'Excelente explicação! Agora entendi a diferença entre gRPC e REST.',
        createdAt: agora
    },
    {
        postId: post1Id.toString(),
        author: 'Aluna Maria',
        content: 'O streaming bidirecional é muito poderoso para aplicações em tempo real.',
        createdAt: agora
    },
    {
        postId: post2Id.toString(),
        author: 'Aluno Pedro',
        content: 'Vale a pena usar protobuf mesmo para projetos pequenos?',
        createdAt: agora
    }
]);

print('✓ Banco blogdb inicializado com dados de exemplo.');
print('  Posts criados: ' + db.posts.countDocuments());
print('  Comentários criados: ' + db.comments.countDocuments());
