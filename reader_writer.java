
import java.util.*;
//EXO1
class RWbasic {
    
    protected int data;
    
    public RWbasic(int num) {
        this.data = num;
    }
    
    public int read() {
        System.out.println("read");
        return this.data;
    }
    
    public void write() {
        System.out.println("write");
        data++;
    }
}

class Reader extends Thread{
    private int nr;
    private RWbasic basic;
    
    public Reader(int _nr, RWbasic _basic) {
        this.nr = _nr;
        this.basic = _basic;
    }
    
    public void run() {
        Random rm = new Random();
        for(int i = 0; i < this.nr; i++) {
            System.out.println("[" + this.basic.read() + "]");
            try {
                Thread.sleep(rm.nextInt(2) * 1000);
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }
    }
}

class Writer extends Thread{
    private int nr;
    private RWbasic basic;
    
    public Writer(int _nr, RWbasic _basic) {
        this.nr = _nr;
        this.basic = _basic;
    }
    
    public void run() {
        Random rm = new Random();
        for(int i = 0; i < this.nr; i++) {
            this.basic.write();
            try {
                Thread.sleep(rm.nextInt(2) * 1000);
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }
    }
}
//EXO2
class RWexclusive extends RWbasic{
    //private int data;
    
    public RWexclusive(int _data) {
        super(_data);
    }
    
    public synchronized int read() {
        System.out.println("synchr read");
        return this.data;
    }
    
    public synchronized void write() {
        System.out.println("synchr write");
        this.data++;
    }
}

//EXO3
class ReaderWriter extends RWbasic{
    private int counter;
    
    private boolean dispo = true;
    
    public ReaderWriter(int _counter, int _data) {
        super(_data);
        this.counter = _counter;
    }
    
    private synchronized void startRead() {
        System.out.println("I am starting reading...");
        this.counter++;
    }
    
    private synchronized void endRead() {
        this.counter--;
        if(this.counter == 0) {
            notify();
        }
    }
    
    private void keepStay() {
        Random rm = new Random();
        try {
            Thread.sleep(rm.nextInt(2) * 1000);
        } catch(InterruptedException ie) {
            ie.printStackTrace();
        }
    }
    
    public int read() {
        startRead();
        System.out.print("I read ");
//        keepStay();
        System.out.println(data);
        endRead();
        dispo = true;
        return this.data;
    }
    
    public synchronized void write() {
        while(this.counter != 0) {
            try {
                wait();
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }
        
        while(dispo == false) {
            try {
                wait();
            } catch (InterruptedException ie) {
                ie.printStackTrace();
            }
        }
        System.out.println("I write!");
        this.data++;
        dispo = false;
    }
}

class Main {

    public Main() {}
    public static void main(String args[]) {
//        RWbasic basic = new RWbasic(0);
//        RWexclusive basic = new RWexclusive(0);
        ReaderWriter basic = new ReaderWriter(0, 0);
        Reader reader = new Reader(10, basic);
        Writer writer = new Writer(10, basic);
        reader.start();
        writer.start();
        
        /*try {
            Thread.sleep(3000);
        } catch(InterruptedException ie) {
            ie.printStackTrace();
        }*/
        
        

        /*try {
            reader.join();
            writer.join();
        } catch (InterruptedException ie) {
            ie.printStackTrace();
        }*/
    }
}






