//plik CzytPis.java

import java.util.Random; //dla losowego czasu dzialania watkow

import java.util.concurrent.Semaphore;

//klasa realizujaca operacje dostepu do czytelni
class Czytelnia {

    private final int N; // pojemnosc czytelni (maksymalna liczba czytelnikow)
    private int czytelnicy; // pomocnicze - aktualna liczba czytelnikow
    private boolean jestPisarz; // pomocnicze - czy w czytelni jest pisarz

    // Note
    static Semaphore writerSem = new Semaphore(1);
    static Semaphore readSem = new Semaphore(1);
    static Semaphore readMax = new Semaphore(1);

    public Czytelnia(int maxN) {
        N = maxN;
        czytelnicy = 0;
        jestPisarz = false;

        System.out.println("Utworzono czytelnie.");
    }

    private void semPost(Semaphore sem) { // Helper function to post on semaphore
        try {
            sem.acquire();
            // System.out.println("Aquired : " + sem.toString());
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    private void semRelease(Semaphore sem) { // Helper function to release semaphore
        try {
            // System.out.println("Released : " + sem.toString());
            sem.release();
        } catch (Exception e) {

        }
    }

    // wywolywane przed czytaniem
    public void wejscieCzytelnika() {

        semPost(readSem);

        czytelnicy++;
        if (czytelnicy == 1) {
            semPost(writerSem);
        }
        semRelease(readSem);
    }

    // wywolywane po zakonczeniu czytania
    public void wyjscieCzytelnika() {

        semPost(readSem);
        czytelnicy--;
        if (czytelnicy == 0) {
            semRelease(writerSem);
        }
        semRelease(readSem);

    }

    // wywolywane przed pisaniem
    public void wejsciePisarza() {
        semPost(writerSem);
    }

    // wywolywane po zakonczeniu pisania
    public void wyjsciePisarza() {
        semRelease(writerSem);
    }
}

// watek czytelnika
class Czytelnik extends Thread {
    Czytelnia czyt;
    String id;

    public Czytelnik(Czytelnia c, String nazwa) {
        czyt = c;
        id = nazwa;
    }

    public void run() {
        Random r = new Random();

        while (true) {
            try {
                sleep(500 + r.nextInt(500)); // losowe opoznienie wejscia
            } catch (InterruptedException e) {
            }

            System.out.println("Czytelnik " + id + " probuje wejsc...");
            czyt.wejscieCzytelnika();

            System.out.println("Czytelnik " + id + " czyta.");
            try {
                sleep(500 + r.nextInt(500)); // losowe opoznienie czytania
            } catch (InterruptedException e) {
            }

            czyt.wyjscieCzytelnika();
            System.out.println("Czytelnik " + id + " wychodzi.");
        }
    }
}

// watek pisarza
class Pisarz extends Thread {
    Czytelnia czyt;
    String id;

    public Pisarz(Czytelnia c, String nazwa) {
        czyt = c;
        id = nazwa;
    }

    public void run() {
        Random r = new Random();

        while (true) {
            try {
                sleep(500 + r.nextInt(500)); // losowe opoznienie wejscia
            } catch (InterruptedException e) {
            }

            System.out.println("Pisarz " + id + " probuje wejsc...");
            czyt.wejsciePisarza();

            System.out.println("Pisarz " + id + " pisze kolejne wiekopomne dzielo.");
            try {
                sleep(800 + r.nextInt(1200)); // losowe opoznienie pisania
            } catch (InterruptedException e) {
            }

            czyt.wyjsciePisarza();
            System.out.println("Pisarz " + id + " wychodzi.");
        }
    }
}

// klasa glowna
public class CzytPis {

    public static void main(String[] arg) {
        Czytelnia czyt = new Czytelnia(2);

        Czytelnik czytelnicy[] = new Czytelnik[] {
                new Czytelnik(czyt, "Kasia"), new Czytelnik(czyt, "Zosia"),
                new Czytelnik(czyt, "Adas"), new Czytelnik(czyt, "Franek")
        };

        Pisarz pisarze[] = new Pisarz[] {
                new Pisarz(czyt, "Adam Mickiewicz"),
                new Pisarz(czyt, "Boleslaw Prus")
        };

        for (int i = 0; i < czytelnicy.length; i++)
            czytelnicy[i].start();
        for (int i = 0; i < pisarze.length; i++)
            pisarze[i].start();

        // uwaga: ponizszy kod ma sens gdyby watki konczyly swoje dzialanie
        try {
            for (int i = 0; i < czytelnicy.length; i++)
                czytelnicy[i].join();
            for (int i = 0; i < pisarze.length; i++)
                pisarze[i].join();
        } catch (InterruptedException e) {
        }
    }
}
