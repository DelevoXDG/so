// Maksim Zdobnikau

import java.util.concurrent.*;
import java.util.concurrent.locks.*;

public class SeaportManagerImpl implements SeaportManager {
	private int docksCount;
	private int seawayCapacity;
	public boolean[] reserved;
	private Semaphore canalPermits;
	private Semaphore docksPermits;
	private Semaphore docksBooked;
	private Lock reservationLock = new ReentrantLock();

	@Override public void init(int numberOfDocks, int seawayCapacity) {
		this.docksCount = numberOfDocks;
		this.seawayCapacity = seawayCapacity;
		this.canalPermits = new Semaphore(seawayCapacity, true);
		this.docksPermits = new Semaphore(docksCount, true);
		this.docksBooked = new Semaphore(docksCount, true);
		this.reserved = new boolean[numberOfDocks];
		for (int i = 0; i < numberOfDocks; i++) {
			this.reserved[i] = false;
		}
	}

	@Override public void requestSeawayEntrance(Ship s) {
		semAcquire(docksBooked, s.getDockingSize());
		semAcquire(canalPermits, 1);
	}

	@Override public int requestPortEntrance(Ship s) {
		semAcquire(docksPermits, s.getDockingSize());
		Integer assignedDock = null;
		assignedDock = this.tryReserve(s.getDockingSize());

		return assignedDock;
	}

	@Override public void signalPortEntered(Ship s) {
		semRelease(canalPermits, 1);
	}

	@Override public void requestPortExit(Ship s) {
		semAcquire(canalPermits, 1);
		semRelease(docksBooked, s.getDockingSize());
	}

	@Override public void signalPortExited(Ship s) {
		this.unReserve(s.getAssignedDock(), s.getDockingSize());
		semRelease(docksPermits, s.getDockingSize());
	}

	@Override public void signalShipSailedAway(Ship s) {
		semRelease(canalPermits, 1);
	}

	public void unReserve(int start, int size) {
		synchronized (reserved) {
			for (int i = start, reservedCount = reserved.length; i < start + size; i++) {
				if (i >= reservedCount) {
					throw new RuntimeException("Unreserving out of bounds: index" + i + "for length " + reservedCount);
				}
				reserved[i] = false;
			}
			reserved.notifyAll();
		}
	}
	public Integer reserveConsecutive(int size) {
		int		start	= 0;
		int		j		= 0;
		boolean	success	= false;
		synchronized (reserved) {
			for (int i = 0, reservedCount = reserved.length; i < reservedCount && j < size; i++) {
				if (reserved[i] == false) {
					start = i;
					success = true;
					while (i + j < reservedCount && j < size) {
						if (reserved[start + j] == true) {
							success = false;
							break;
						}
						j++;
					}
					if (j == size) {
						for (int k = start; k < start + size; k++) {
							reserved[k] = true;
						}
						return start;
					} else {
						success = false;
						i = start + j;
						j = 0;
					}
				}
			}
		}
		if (success == false) {
			return null;
		}
		return start;
	}

	// private int reserve(int size) {
	// 	int	start	= 0;
	// 	int	j		= 0;
	// 	synchronized (reserved) {
	// 		for (int i = 0; i < docksCount && j < size; i++) {
	// 			if (reserved[i] == false) {
	// 				start = i;
	// 				while (j < size) {
	// 					reserved[start + j] = true;
	// 					j++;
	// 				}
	// 			}
	// 		}
	// 	}
	// 	return start;
	// }
	private int tryReserve(int size) {
		Integer assignedDock = null;
		reservationLock.lock();
		synchronized (reserved) {
			try {
				assignedDock = this.reserveConsecutive(size);
				while (assignedDock == null) {
					reserved.wait();
					assignedDock = this.reserveConsecutive(size);
				}
			} catch (InterruptedException e) {

			} finally {
				reservationLock.unlock();
			}
		}
		return assignedDock;
	}
	private void semAcquire(Semaphore sem, int count) {
		try {
			sem.acquire(count);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private void semAcquire(Semaphore sem) {
		semAcquire(sem, 1);
	}
	private void semRelease(Semaphore sem, int count) {
		try {
			// System.out.println("Released : " + sem.toString());
			sem.release(count);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private void semRelease(Semaphore sem) {
		semRelease(sem, 1);
	}
}
