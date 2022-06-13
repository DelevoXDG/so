
// Maksim Zdobnikau
import java.util.concurrent.*;
import java.util.concurrent.locks.*;

public class SeaportManagerImpl implements SeaportManager {
	private int docksCount;
	private int seawayCapacity;
	private int seawayCount;
	private int seawayPut;
	private int seawayTake;
	public boolean[] reserved;
	private Semaphore canalPermits;
	private Semaphore docksPermits;
	private Semaphore docksBooked;
	private Semaphore arrSem = new Semaphore(1);
	private Lock lock = new ReentrantLock();
	final Condition hasConsecutiveDocks = lock.newCondition();
	Lock seawayLock = new ReentrantLock();
	// final Condition seawayNotFull = seawayLock.newCondition();
	// Semaphore acquire(int permits1, int permits2, Semaphore semaphore1, Semaphore semaphore2)
	// 		throws InterruptedException {
	// 	Integer		candidate	= 0;
	// 	int[]		permits		= new int[] { permits1, permits1 };
	// 	Semaphore[]	semaphores	= new Semaphore[] { semaphore1, semaphore2 };

	// 	while (true) { // polling loop
	// 		for (int i = 0; i < semaphores.length; i++) {
	// 			// attempt to aquire from next Semaphore in the list
	// 			if (semaphores[i].tryAcquire(permits[i])) {
	// 				return semaphores[i];
	// 			}
	// 			// choose the candidate semaphore with maximum available permits
	// 			if (candidate == null || semaphores[candidate].availablePermits() < semaphores[i].availablePermits()) {
	// 				candidate = i;
	// 			}
	// 		}
	// 		// now we have to wait some time
	// 		// instead of plain sleeping, we wait on the most filled semaphore 
	// 		if (semaphores[candidate].tryAcquire(permits[candidate], 10, TimeUnit.MILLISECONDS)) {
	// 			return semaphores[candidate];
	// 		}
	// 	}
	// }
	@Override public void init(int numberOfDocks, int seawayCapacity) {
		this.docksCount = numberOfDocks;
		this.seawayCapacity = seawayCapacity;
		this.seawayCount = 0;
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
		// seawayLock.lock();
		// synchronized (this) {
		// 	try {
		// 		while (s.getDockingSize() > docksBooked.availablePermits()) {
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
		// semAcquire(arrSem, 1);
		Integer assignedDock = null;
		while (assignedDock == null) {
			assignedDock = this.reserve(s.getDockingSize());
		}
		// semRelease(arrSem, 1);

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
		// semAcquire(arrSem, 1);
		this.unReserve(s.getAssignedDock(), s.getDockingSize());
		semRelease(docksPermits, s.getDockingSize());
		// semRelease(arrSem, 1);
	}

	@Override public void signalShipSailedAway(Ship s) {
		semRelease(canalPermits, 1);
	}
	private void semAcquire(Semaphore sem) {
		try {
			sem.acquire();
			// System.out.println("Aquired : " + sem.toString());
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private void semAcquire(Semaphore sem, int count) {
		try {
			sem.acquire(count);
			// System.out.println("Aquired : " + sem.toString());
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}

	private void semRelease(Semaphore sem) {
		try {
			// System.out.println("Released : " + sem.toString());
			sem.release();
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private void semRelease(Semaphore sem, int count) {
		try {
			// System.out.println("Released : " + sem.toString());
			sem.release(count);
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}
	}
	private int reserve(int size) {
		int	start	= 0;
		int	j		= 0;
		synchronized (reserved) {
			for (int i = 0; i < docksCount && j < size; i++) {
				if (reserved[i] == false) {
					start = i;
					while (j < size) {
						reserved[start + j] = true;
						j++;
					}
				}
			}
		}
		return start;
	}
	public void unReserve(int start, int size) {
		synchronized (reserved) {
			for (int i = start, reservedCount = reserved.length; i < start + size; i++) {
				if (i >= reservedCount) {
					throw new RuntimeException("Unreserving out of bounds: index" + i + "for length " + reservedCount);
				}
				reserved[i] = false;
			}
		}
		reserved.notifyAll();
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
						// reserved[start + j] = true;
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
	private int tryReserve(int size) {
		Integer assignedDock = null;
		lock.lock();
		synchronized (reserved) {
			try {
				assignedDock = this.reserveConsecutive(size);
				while (assignedDock == null) {
					reserved.wait();
					assignedDock = this.reserveConsecutive(size);
				}
			} catch (InterruptedException e) {

			} finally {
				lock.unlock();
			}
		}

		// try {
		// 	while (count == items.length)
		// 		notFull.await();
		// 	items[putptr] = x;
		// 	if (++putptr == items.length) putptr = 0;
		// 	++count;
		// 	notEmpty.signal();
		// } finally {
		// 	lock.unlock();
		// }
		return assignedDock;
	}
}
