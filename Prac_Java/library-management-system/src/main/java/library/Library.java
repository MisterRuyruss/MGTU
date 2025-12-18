package library;

import java.util.ArrayList;
import java.util.List;

public class Library {
    private List<Book> books;
    
    
    public static class OperationLog {
        
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
    
    }
}