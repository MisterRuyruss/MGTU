public class Main {
    
    public static void main(String[] args) {
        Product laptop = new Product("Ноутбук", 75000.0);
        Product mouse = new Product("Мышка", 1500.0);
        Product keyboard = new Product("Клавиатура", 3000.0);
        Product headphones = new Product("Наушники", 5000.0);
        Product monitor = new Product("Монитор", 25000.0);

        Cart cart = new Cart();

        System.out.println("Добавляем товары в корзину...");
        cart.addProduct(laptop);
        cart.addProduct(mouse);
        cart.addProduct(keyboard);
        cart.addProduct(headphones);

        System.out.println("\n=== Первый показ корзины ===");
        cart.printCart();

        System.out.println("\nДобавляем Монитор...");
        cart.addProduct(monitor);

        System.out.println("\n=== Корзина после добавления монитора ===");
        cart.printCart();

        System.out.println("\nУдаляем товар: Мышка");
        cart.removeProduct("Мышка");

        System.out.println("\n=== Корзина после удаления ===");
        cart.printCart();

        System.out.println("\nУдаляем товар: Наушники");
        cart.removeProduct("Наушники");

        System.out.println("\n=== Корзина на данный момент ===");
        cart.printCart();
    }
}