
public interface SeaportManager {

    void init(int numberOfDocks, int seawayCapacity);

    void requestSeawayEntrance(Ship s);

    int requestPortEntrance(Ship s);

    void signalPortEntered(Ship s);

    void requestPortExit(Ship s);

    void signalPortExited(Ship s);

    void signalShipSailedAway(Ship s);
}
