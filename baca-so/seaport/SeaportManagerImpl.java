
// Maksim Zdobnikau

// Napisz klasę implementującą interfejs SeaportManager o nazwie SeaportManagerImpl, która będzie właściwie zarządzała ruchem w porcie. Osiągnij to poprzez
// odpowiednie blokowanie wątku w metodach z interfejsu SeaportManager. Każdy statek zaimplementowany jest jako wątek, który wywołuje kolejno metody:

// requestSeawayEntrance() - wywoływana przy zbliżaniu się do kanału; statek zacznie wpływać do kanału po powrocie z tej metody.
// requestPortEntrance() - wywoływana przy zbliżaniu się do portu; statek zacznie wpływać do portu po powrocie z tej metody; liczba całkowita zwrócona przez tą
// metodę określa najniższy numer doku spośród doków, do których statek zacumuje.
// signalPortEntered() - wywoływana po opuszczeniu kanału (podczas wpływania do portu).
// requestPortExit() - wywoływana przed opuszczeniem portu; statek zacznie wpływać do kanału po powrocie z tej metody.
// signalPortExited() - wywoływana po opuszczeniu portu.
// signalShipSailedAway() - wywoływana tuż po opuszczeniu kanału (podczas wypływania na otwarte morze).
// Twój program musi spełnić następujące warunki:

// W kanale może się znaleźć maksymalnie M statków jednocześnie.
// Dany dok może być zajmowany przez maksymalnie jeden statek jednocześnie.
// Numer najniższego doku, do którego statek ma zacumować, musi zawierać się w poprawnym zakresie.
// Zasoby muszą być wykorzystywane optymalnie (równoległość implementacji).
// Każdy statek musi zakończyć swoją podróż (brak zakleszczeń).
// Dodatkowe informacje:

// Jako pierwsza zostanie wywołana metoda init() poprzez którą Twoja klasa otrzyma parametry portu (liczba doków, pojemność kanału).
// Metoda requestPortEntrance() zwraca numer wyznaczający doki, do których statek zacumuje. Jest to najniższy numer doku spośród doków, do których statek
// zacumuje. Doki są numerowane od zera. Przykładowo, jeśli rozmiar statku wynosi k = 3 i metoda zwróci liczbę 1, to statek zacumuje do doków numer 1, 2 oraz 3.
// Statek będzie pamiętał ten numer po przypisaniu i możesz go sprawdzić metodą getAssignedDock() interfejsu Ship.
// Rozmiar statku sprawdzisz metodą getDockingSize() interfejsu Ship.
// Nie ma żadnych innych gwarancji co do wątków poza kolejnością wywołań metod - każdy wątek może dowolnie dobierać czas pomiędzy wspomnianymi wywołaniami.

import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.concurrent.atomic.*;

public class SeaportManagerImpl implements SeaportManager {
	int docksCount;
	int seawayCapacity;
	int seawayCount;
	int seawayPut;
	int seawayTake;
	boolean[] docksTaken;
	Semaphore entranceLock = new Semaphore(1);
	Lock seawayLock = new ReentrantLock();
	final Condition seawayNotFull = seawayLock.newCondition();
	final Condition seawayNotEmpty = seawayLock.newCondition();

	@Override public void init(int numberOfDocks, int seawayCapacity) {
		this.docksCount = numberOfDocks;
		this.seawayCapacity = seawayCapacity;
		this.seawayCount = 0;
		this.docksTaken = new boolean[numberOfDocks];
	}

	@Override public void requestSeawayEntrance(Ship s) {
		seawayLock.lock();
		try {
			while (seawayCount == seawayCapacity) {
				try {
					seawayNotFull.await();
				} catch (InterruptedException e) {
					return;
				}
				if (++seawayPut == seawayCount) {
					seawayPut = 0;
				}
				seawayCount++;
				seawayNotEmpty.signal();
			}
		} finally {
			seawayLock.unlock();
		}
	}

	@Override public int requestPortEntrance(Ship s) {

		// TODO Auto-generated method stub
		return 0;
	}

	@Override public void signalPortEntered(Ship s) {
		// TODO Auto-generated method stub

	}

	@Override public void requestPortExit(Ship s) {
		// TODO Auto-generated method stub

	}

	@Override public void signalPortExited(Ship s) {
		// TODO Auto-generated method stub

	}

	@Override public void signalShipSailedAway(Ship s) {
		// TODO Auto-generated method stub

	}

}
