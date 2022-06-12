// Maksim Zdobnikau

import java.util.concurrent.*;
import java.util.concurrent.locks.*;

public class SeaportManagerImpl implements SeaportManager {
	int docksCount;
	int seawayCapacity;
	int seawayCount;
	int seawayPut;
	int seawayTake;
	boolean[] reserved;
	Semaphore canalPermits;
	Semaphore docksPermits;
	Semaphore docksBooked;
	Semaphore arrSem = new Semaphore(1);
	// Lock seawayLock = new ReentrantLock();
	// final Condition seawayNotFull = seawayLock.newCondition();
	// final Condition seawayNotEmpty = seawayLock.newCondition();

	@Override public void init(int numberOfDocks, int seawayCapacity) {
		this.docksCount = numberOfDocks;
		this.seawayCapacity = seawayCapacity;
		this.seawayCount = 0;
		this.canalPermits = new Semaphore(seawayCapacity);
		this.docksPermits = new Semaphore(docksCount);
		this.docksBooked = new Semaphore(docksCount);
		this.reserved = new boolean[numberOfDocks];
		for (int i = 0; i < numberOfDocks; i++) {
			this.reserved[i] = false;
		}
	}

	@Override public void requestSeawayEntrance(Ship s) {
		semAcquire(docksBooked, s.getDockingSize());
		semAcquire(canalPermits, 1);

		// seawayLock.lock();
		// synchronized (this) {
		// 	try {
		// 		while (s.getDockingSize() > bookedDocks.availablePermits()) {
		// 			semRelease(entranceLock);
		// 			try {
		// 				seawayNotFull.await();
		// 			} catch (InterruptedException e) {
		// 				return;
		// 			}
		// 			semAcquire(entranceLock);
		// 		}
		// 	} finally {
		// 		seawayLock.unlock();
		// 	}
		// }
	}

	@Override public int requestPortEntrance(Ship s) {
		semAcquire(docksPermits, s.getDockingSize());
		semAcquire(arrSem, 1);
		int place = this.reserve(s.getDockingSize());
		semRelease(arrSem, 1);

		return place;
	}

	@Override public void signalPortEntered(Ship s) {
		semRelease(canalPermits, 1);
	}

	@Override public void requestPortExit(Ship s) {
		semAcquire(canalPermits, 1);
		semRelease(docksBooked, s.getDockingSize());
	}

	@Override public void signalPortExited(Ship s) {
		semRelease(docksPermits, s.getDockingSize());
		semAcquire(arrSem, 1);
		this.unReserve(s.getAssignedDock(), s.getDockingSize());
		semRelease(arrSem, 1);
	}

	@Override public void signalShipSailedAway(Ship s) {
		semRelease(canalPermits, 1);
	}
	private void semAcquire(Semaphore sem) { // Helper function to post on semaphore
		try {
			sem.acquire();
			// System.out.println("Aquired : " + sem.toString());
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private void semAcquire(Semaphore sem, int count) { // Helper function to post on semaphore
		try {
			sem.acquire(count);
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
			System.out.println(e.getMessage());
		}
	}
	private void semRelease(Semaphore sem, int count) { // Helper function to release semaphore
		try {
			// System.out.println("Released : " + sem.toString());
			sem.release(count);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private synchronized int reserve(int size) {
		int	start	= 0;
		int	j		= 0;
		for (int i = 0; i < docksCount && j < size; i++) {
			if (reserved[i] == false) {
				start = i;
				while (j < size) {
					reserved[start + j] = true;
					j++;
				}
			}
		}
		return start;
	}
	private synchronized void unReserve(int start, int size) {
		for (int i = start; i < start + size; i++) {
			reserved[i] = false;
		}

	}
}
