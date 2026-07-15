#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class Product{
public:
    virtual std::string getName() const = 0;
    virtual double getPrice() const = 0;
    virtual ~Product() = default;
};

class Laptop : public Product
{
private:
    double price;
    std::string serialNumber;
public:
    Laptop(double price, const std::string& serialNumber)
        : price(price), serialNumber(serialNumber)
    {}

    std::string getName() const override
    {
        return "Laptop";
    }

    double getPrice() const override
    {
        return price;
    }

    std::string getSerialNumber() const
    {
        return serialNumber;
    }
};

class Mouse : public Product
{
private:
    double price;
public:
    Mouse(double price)
        : price(price)
    {}

    std::string getName() const override
    {
        return "Mouse";
    }

    double getPrice() const override
    {
        return price;
    }
};

class GameLicense : public Product
{
private:
    double price;
    std::string activationKey;
public:
    GameLicense(double price, const std::string& activationKey)
        : price(price), activationKey(activationKey)
    {}

    std::string getName() const override
    {
        return "GameLicense";
    }

    double getPrice() const override
    {
        return price;
    }

    std::string getActivationKey() const
    {
        return activationKey;
    }
};

class Customer
{
private:
    std::string name;
    bool vatPayer;
public:
    Customer(const std::string& name, bool vatPayer)
        : name(name), vatPayer(vatPayer)
    {}

    std::string getName() const
    {
        return name;
    }

    bool isVatPayer() const
    {
        return vatPayer;
    }
};

class OrderItem
{
private:
    Product* product;
    int quantity;
public:
    OrderItem(Product* product, int quantity)
        : product(product), quantity(quantity)
    {}

    Product* getProduct() const
    {
        return product;
    }

    int getQuantity() const
    {
        return quantity;
    }
};

class TaxCalculator
{
private:
    double vatPercent;
    double laptopCustomsFee;
public:
    TaxCalculator(double vatPercent, double laptopCustomsFee)
        : vatPercent(vatPercent), laptopCustomsFee(laptopCustomsFee){}

    double getVatPercent() const
    {
        return vatPercent;
    }

    double getLaptopCustomsFee() const
    {
        return laptopCustomsFee;
    }

    double calculateItemPrice(double productPrice, int quantity, bool isVatPayer, bool hasCustomsFee) const
    {
        double unitPrice = productPrice;
        if (isVatPayer)
        {
            unitPrice += productPrice * vatPercent / 100.0;
        }
        if (hasCustomsFee)
        {
            unitPrice += laptopCustomsFee;
        }
        return unitPrice * quantity;
    }

    double calculateTotal(const std::vector<double>& itemPrices) const
    {
        double total = 0.0;
        for (double price : itemPrices)
        {
            total += price;
        }
        return total;
    }
};

class Order
{
private:
    const Customer& customer;
    std::vector<OrderItem> items;
public:
    Order(const Customer& customer)
        : customer(customer){}

    void addItem(Product* product, int quantity)
    {
        if (product == nullptr || quantity <= 0)
        {
            return;
        }
        items.emplace_back(product, quantity);
    }

    void printInvoice(const TaxCalculator& taxCalculator) const
    {
        std::vector<double> itemPrices;
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "Customer: " << customer.getName() << '\n';
        for (const OrderItem& item : items)
        {
            Product* product = item.getProduct();
            Laptop* laptop = dynamic_cast<Laptop*>(product);
            GameLicense* game = dynamic_cast<GameLicense*>(product);
            bool hasCustomsFee = laptop != nullptr;
            double itemPrice = taxCalculator.calculateItemPrice(
                product->getPrice(),
                item.getQuantity(),
                customer.isVatPayer(),
                hasCustomsFee
            );
            itemPrices.push_back(itemPrice);
            std::cout << "-- " << product->getName();
            if (laptop != nullptr)
            {
                std::cout << " (SN: " << laptop->getSerialNumber() << ")";
            }
            else if (game != nullptr)
            {
                std::cout << " (Key: " << game->getActivationKey() << ")";
            }
            if (item.getQuantity() > 1)
            {
                std::cout << " (x" << item.getQuantity() << ")";
            }
            std::cout << " | Price (VAT ";
            if (customer.isVatPayer())
            {
                std::cout << taxCalculator.getVatPercent();
            }
            else
            {
                std::cout << 0.0;
            }
            std::cout << "%";
            if (hasCustomsFee)
            {
                std::cout << " + Customs " << taxCalculator.getLaptopCustomsFee();
            }
            std::cout << "): " << itemPrice << '\n';
        }
        double totalDue = taxCalculator.calculateTotal(itemPrices);
        std::cout << "Total due: " << totalDue << '\n';
    }
};

int main()
{
    TaxCalculator taxCalc(20, 50);
    Product* laptop = new Laptop(1000.0, "SN-ASUS-2026X");
    Product* game = new GameLicense(50.0, "X9RT-Z67Y-PL12");
    Customer customer("Ivan", true);
    Order order(customer);
    order.addItem(laptop, 1);
    order.addItem(game, 1);
    order.printInvoice(taxCalc);
    delete laptop;
    delete game;
    return 0;
}