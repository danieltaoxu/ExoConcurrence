import java.util.*;
import java.util.concurrent.Semaphore;


class Client extends Thread {
    
    private int id;
    private Semaphore semCabine;
    private Semaphore semPanier;
    
    public Client(Semaphore _cabine, Semaphore _panier, int _id) {
        this.semCabine = _cabine;
        this.semPanier = _panier;
        this.id = _id;
    }
    
    public void run() {
        Random rm = new Random();
        try {
            System.out.println(this.id + " am waitting for the key of cabine.");
            semCabine.acquire();
            System.out.println(this.id + " ask for the empty basket.");
            semPanier.acquire();
            //change closes
            Thread.sleep(rm.nextInt(5) * 1000);
            semCabine.release();
            System.out.println(this.id + " return the cabine.");
            System.out.println(this.id + " store the basket.");
            
            //swim
            Thread.sleep(rm.nextInt(3) * 1000);
            System.out.println(this.id + " finished swimming, want a cabine.");
            semCabine.tryAcquire();
            Thread.sleep(rm.nextInt(3) * 1000);
            semCabine.release();
            semPanier.release();
            System.out.println(this.id + " leaves the piscine.");
        } catch (InterruptedException ie) {
            ie.printStackTrace();
        }
    }
}

class Main {
    public static void main(String[] args) {
        int quantity_cabine = 3, quantity_panier = 5, quantity_client = 10;
        Semaphore cabine = new Semaphore(quantity_cabine);
        Semaphore panier = new Semaphore(quantity_panier);
        
        Thread clients[] = new Client[quantity_client];
        
        for(int i = 0; i < quantity_client; i++) {
            clients[i] = new Client(cabine, panier, i);
            clients[i].start();
        }
    }
}