
import java.util.ArrayList;

public class Cart {
    private ArrayList<Product> products;

    public Cart() {
        products = new ArrayList<>();
    }

    public void addProduct(Product p) {
        products.add(p);
    }

    public void removeProduct(String name) {
        products.removeIf(p -> p.getName().equalsIgnoreCase(name));
    }

    public double getTotal() {
        double total = 0;
        for (Product p : products) {
            total += p.getPrice();
        }
        return total;
    }

    public void printCart() {
        System.out.println("=== Корзина ===");
        if (products.isEmpty()) {
            System.out.println("Корзина пуста");
        } else {
            for (int i = 0; i < products.size(); i++) {
                System.out.println((i + 1) + ". " + products.get(i));
            }
            System.out.println("Итого: " + getTotal() + " руб");
        }
    }
}