package com.blog.server.config;

import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoDatabase;

import java.util.logging.Logger;

/**
 * Configuração e gerenciamento da conexão com o MongoDB.
 *
 * Utiliza o padrão Singleton para garantir que apenas uma instância
 * de MongoClient exista durante o ciclo de vida da aplicação.
 * O MongoClient é thread-safe e caro de instanciar — deve ser reutilizado.
 *
 * A URI e o nome do banco são lidos de variáveis de ambiente,
 * com valores padrão para desenvolvimento local com Docker.
 */
public class MongoDBConfig {

    private static final Logger logger = Logger.getLogger(MongoDBConfig.class.getName());

    // Variáveis de ambiente lidas em tempo de execução
    // Padrões apontam para o container Docker definido no docker-compose.yml
    private static final String MONGO_URI  = System.getenv().getOrDefault(
            "MONGO_URI", "mongodb://root:senha123@localhost:27017");
    private static final String DB_NAME    = System.getenv().getOrDefault(
            "MONGO_DB", "blogdb");

    // Instância Singleton — volatile garante visibilidade entre threads
    private static volatile MongoDBConfig instance;

    private final MongoClient   mongoClient;
    private final MongoDatabase database;

    /** Construtor privado: inicializa a conexão com o MongoDB. */
    private MongoDBConfig() {
        logger.info("Conectando ao MongoDB: " + MONGO_URI);
        this.mongoClient = MongoClients.create(MONGO_URI);
        this.database    = mongoClient.getDatabase(DB_NAME);
        logger.info("Conexão estabelecida com o banco: " + DB_NAME);
    }

    /**
     * Retorna a instância única (Double-Checked Locking).
     * Thread-safe sem sincronização desnecessária após a primeira criação.
     */
    public static MongoDBConfig getInstance() {
        if (instance == null) {
            synchronized (MongoDBConfig.class) {
                if (instance == null) {
                    instance = new MongoDBConfig();
                }
            }
        }
        return instance;
    }

    /** Retorna o banco de dados configurado. */
    public MongoDatabase getDatabase() {
        return database;
    }

    /** Fecha a conexão — chamar ao encerrar a aplicação. */
    public void close() {
        if (mongoClient != null) {
            mongoClient.close();
            logger.info("Conexão com MongoDB encerrada.");
        }
    }
}
