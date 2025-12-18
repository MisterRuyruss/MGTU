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

    // Методы библиотеки (ПОЛНАЯ РЕАЛИЗАЦИЯ)
    public void addBook(Book book) {
        books.add(book);
        operationLog.addEntry(OperationLog.OperationType.ADD_BOOK,
            "Добавлена книга: " + book.getTitle());
    }

    public Book findBookById(int id) {
        for (Book book : books) {
            if (book.getId() == id) {
                return book;
            }
        }
        return null;
    }

    public List<Book> findBooksByAuthor(String author) {
        List<Book> result = new ArrayList<>();
        for (Book book : books) {
            if (book.getAuthor().equalsIgnoreCase(author)) {
                result.add(book);
            }
        }
        return result;
    }

    public boolean borrowBook(int id) {
        Book book = findBookById(id);
        if (book != null && book.isAvailable()) {
            book.setAvailable(false);
            operationLog.addEntry(OperationLog.OperationType.BORROW,
                "Выдана книга ID: " + id + " - " + book.getTitle());
            return true;
        }
        return false;
    }

    public boolean returnBook(int id) {
        Book book = findBookById(id);
        if (book != null && !book.isAvailable()) {
            book.setAvailable(true);
            operationLog.addEntry(OperationLog.OperationType.RETURN,
                "Возвращена книга ID: " + id + " - " + book.getTitle());
            return true;
        }
        return false;
    }

    public List<Book> getAvailableBooks() {
        List<Book> result = new ArrayList<>();
        for (Book book : books) {
            if (book.isAvailable()) {
                result.add(book);
            }
        }
        return result;
    }

    public void printOperationLog() {
        operationLog.printLog();
    }

    public String getStatistics() {
        int total = books.size();
        int available = 0;
        for (Book book : books) {
            if (book.isAvailable()) {
                available++;
            }
        }
        int borrowed = total - available;
        
        return String.format("Всего книг: %d, Доступно: %d, Выдано: %d", 
                            total, available, borrowed);
    }

        
    public boolean removeBook(int id) {
        Book book = findBookById(id);
        if (book != null) {
            books.remove(book);
            operationLog.addEntry(OperationLog.OperationType.ADD_BOOK,
                "Удалена книга ID: " + id + " - " + book.getTitle());
            return true;
        }
        return false;
    }
    
    public boolean updateBook(int id, Book newData) {
        Book book = findBookById(id);
        if (book != null) {
            operationLog.addEntry(OperationLog.OperationType.ADD_BOOK,
                "Обновлена книга ID: " + id);
            return true;
        }
        return false;
    }
}