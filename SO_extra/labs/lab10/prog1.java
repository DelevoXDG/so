import java.util.concurrent.Semaphore;

class Watek extends Thread {
	final int num;

	Watek(int num) {
		this.num = num;
	}

	private int signal = 0;

	public synchronized void take() {
		this.signal++;
		this.notify();
	}

	public synchronized void release() throws InterruptedException {
		while (this.signal == 0)
			wait();
		this.signal--;
	}

	public void run() {
		System.out.println("Thread " + num + " started");
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			System.out.println(e.getMessage());
		}
	}

}

public class prog1 {

	// class Watek implements Runnable
	// {

	// }

	public static void main(String[] args) {
		final int threadCount = 20;
		Watek[] threads = new Watek[threadCount];
		for (int i = 0; i < threadCount; i++) {
			threads[i] = new Watek(i);
		}

		// threads[2].start();
		// threads[1].start();
		// threads[0].start();
		for (int i = 0; i < threadCount; i++) {
			threads[i].start();
			// try {
			// threads[i].join();
			// } catch (InterruptedException e) {
			// System.out.println(e.getMessage());
			// }

		}
		for (int i = 0; i < threadCount; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				System.out.println(e.getMessage());
			}
		}
	}
}

// klasa Watek
