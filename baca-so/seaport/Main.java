import java.util.Random;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class ShipThread extends Thread implements Ship {
    int size;
    Integer assignedDock;
    SeaportManager manager;
    int id;

    ShipThread(int id, int size, SeaportManager manager) {
        this.size = size;
        this.manager = manager;
        this.id = id;
    }

    public int getDockingSize() {
        return size;
    }

    public Integer getAssignedDock() {
        return assignedDock;
    }

    public void run() {
        System.out.format("%d [%d] %s: requestSeawayEntrance\n", id, getDockingSize(), getAssignedDock());
        Random r = new Random();
        manager.requestSeawayEntrance(this);
        System.out.format("%d [%d] %s: requestPortEntrance\n", id, getDockingSize(), getAssignedDock());
        assignedDock = manager.requestPortEntrance(this);
        System.out.format("%d [%d] %s: signalPortEntered ( %s )\n", id, getDockingSize(), getAssignedDock(),
                getAssignedDock());
        manager.signalPortEntered(this);
        System.out.format("%d [%d] %s: idling\n", id, getDockingSize(), getAssignedDock());
        try {
            // Thread.sleep(new Random().nextInt(20));
            // if (true && r.nextInt(5) % 2 == 0) Thread.sleep(r.nextInt(20));
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }
        System.out.format("%d [%d] %s: requestPortExit\n", id, getDockingSize(), getAssignedDock());
        manager.requestPortExit(this);
        System.out.format("%d [%d] %s: signalPortExited\n", id, getDockingSize(), getAssignedDock());
        manager.signalPortExited(this);
        System.out.format("%d [%d] %s: signalShipSailedAway\n", id, getDockingSize(), getAssignedDock());
        manager.signalShipSailedAway(this);
        System.out.format("%d [%d] %s: done\n", id, getDockingSize(), getAssignedDock());
    }
}

class Main {
    public static Scanner sc = new Scanner(System.in);
    public static void main(String[] args) {
        Random         r       = new Random();
        // SeaportManager manager = new SeaportManagerImpl();
        SeaportManager manager = new HL();
        manager.init(5, 5);
        FileWriter        file        = null;
        // try {
        //     file = new FileWriter("/mnt/c/Users/ASUS/.delevo/repos/so/baca-so/seaport/tests/test.00.in");
        // } catch (IOException e) {

        // }
        String            newLine     = System.getProperty("line.separator");
        ArrayList<Thread> threadList  = new ArrayList<Thread>();
        int               threadCount = sc.nextInt();
        for (int shipId = 0; shipId < threadCount; shipId++) {
            try {
                // int    size = r.nextInt(5 - 1) + 1;
                Thread s = new ShipThread(sc.nextInt(), sc.nextInt(), manager);
                // try {
                // file.write(shipId + ": " + size + newLine);
                // } catch (IOException e) {
                // }
                threadList.add(s);
                s.start();
                // if (true && r.nextInt(5) % 2 == 0) Thread.sleep(r.nextInt(1));
                Thread.sleep(5);
            } catch (InterruptedException e) {
            }
        }
        for (Thread th : threadList) {
            try {
                th.join();
            } catch (InterruptedException e) {
            }
        }
        // try {
        //     file.close();
        // } catch (IOException e) {
        // }

    }

}
