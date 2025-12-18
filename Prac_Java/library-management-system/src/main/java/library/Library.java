package library;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

public class Library {
    private List<Book> books;
    
    public static class OperationLog {
        public class LogEntry {
            private OperationType type;
            private LocalDateTime timestamp;
            private String description;

            public LogEntry(OperationType type, String description) {
                this.type = type;
                this.timestamp = LocalDateTime.now();
                this.description = description;
            }

            public OperationType getType() { return type; }
            public LocalDateTime getTimestamp() { return timestamp; }
            public String getDescription() { return description; }

            @Override
            public String toString() {
                return String.format("[%s] %s - %s", 
                    timestamp, type, description);
            }
        }

        public enum OperationType {
            ADD_BOOK, BORROW, RETURN
        }

        private List<LogEntry> entries;

        public OperationLog() {
            this.entries = new ArrayList<>();
        }

        public void addEntry(OperationType type, String description) {
            entries.add(new LogEntry(type, description));
        }

        public List<LogEntry> getEntries() {
            return new ArrayList<>(entries);
        }

        public void printLog() {
            if (entries.isEmpty()) {
                System.out.println("Журнал операций пуст.");
                return;
            }
            System.out.println("=== ЖУРНАЛ ОПЕРАЦИЙ ===");
            for (LogEntry entry : entries) {
                System.out.println(entry);
            }
            System.out.println("======================");
        }
    }
    
    private OperationLog operationLog;

    public Library() {
        this.books = new ArrayList<>();
        this.operationLog = new OperationLog();
    }

    public void addBook(Book book) {
        
    }

    public Book findBookById(int id) {
        return null;
    }

    public List<Book> findBooksByAuthor(String author) {
        return new ArrayList<>();
    }

    public boolean borrowBook(int id) {
        return false;
    }

    public boolean returnBook(int id) {
        return false;
    }

    public List<Book> getAvailableBooks() {
        return new ArrayList<>();
    }

    public void printOperationLog() {
        operationLog.printLog();
    }
}