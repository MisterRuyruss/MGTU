package library;

import java.util.List;

public class Main {
    public static void main(String[] args) {
        Library library = new Library();

        System.out.println("=== ДОБАВЛЕНИЕ КНИГ ===");
        library.addBook(new Book(1, "Война и мир", 
            "Л.Н. Толстой", 1869, "978-5-17-090335-2"));
        library.addBook(new Book(2, "Преступление и наказание", 
            "Ф.М. Достоевский", 1866, "978-5-17-090336-9"));
        library.addBook(new Book(3, "Анна Каренина", 
            "Л.Н. Толстой", 1877, "978-5-17-090337-6"));
        System.out.println("Книги добавлены.\n");

        System.out.println("=== ПОИСК КНИГИ ПО ID ===");
        Book book = library.findBookById(2);
        if (book != null) {
            System.out.println("Найдена: " + book);
        } else {
            System.out.println("Книга не найдена.");
        }

        System.out.println("\n=== ПОИСК КНИГ ПО АВТОРУ ===");
        List<Book> tolstoyBooks = library.findBooksByAuthor("Л.Н. Толстой");
        System.out.println("Книги Толстого: " + tolstoyBooks.size() + " шт.");
        for (Book b : tolstoyBooks) {
            System.out.println("  - " + b.getTitle());
        }

        System.out.println("\n=== ВЫДАЧА КНИГИ ===");
        boolean borrowed = library.borrowBook(1);
        System.out.println("Книга ID=1 " + (borrowed ? "выдана успешно" : "не выдана (уже занята или не найдена)"));

        System.out.println("\n=== ДОСТУПНЫЕ КНИГИ ===");
        List<Book> availableBooks = library.getAvailableBooks();
        System.out.println("Доступно книг: " + availableBooks.size());
        for (Book b : availableBooks) {
            System.out.println("  - " + b.getTitle() + " (ID: " + b.getId() + ")");
        }

        System.out.println("\n=== ВОЗВРАТ КНИГИ ===");
        boolean returned = library.returnBook(1);
        System.out.println("Книга ID=1 " + (returned ? "возвращена" : "не возвращена (не была выдана)"));

        System.out.println("\n=== ЖУРНАЛ ОПЕРАЦИЙ ===");
        library.printOperationLog();
    }
}