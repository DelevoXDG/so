
import static org.junit.Assert.*;
import org.junit.Test;

import java.util.Arrays;
// import sorts.*;

public class Tests {

	static boolean[] getBool(int[] arr) {
		boolean[] result = new boolean[arr.length];
		for (int i = 0; i < arr.length; i++) {
			result[i] = arr[i] == 1 ? true : false;
			;
		}
		return result;
	}

	static String intToString(int[] arr) {
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < arr.length; i++) {
			sb.append(arr[i]).append("");
		}
		sb.setLength(Math.max(sb.length() - 1, 0));
		return sb.toString();
	}
	static String boolToString(boolean[] arr) {
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < arr.length; i++) {
			sb.append(arr[i] == true ? "1" : "0").append("");
		}
		sb.setLength(Math.max(sb.length() - 1, 0));
		return sb.toString();
	}

	@Test public void reserveShips() {
		int					N			= 10;
		boolean[]			arr			= new boolean[N];
		SeaportManagerImpl	manager		= new SeaportManagerImpl();
		Integer				expected	= null;

		manager.init(N, 5);
		manager.reserved = getBool(new int[] { 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1 });
		assertEquals(null, manager.reserveConsecutive(5));

		expected = 7;
		assertEquals(expected, manager.reserveConsecutive(3));
		assertEquals(intToString(new int[] { 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1 }), boolToString(manager.reserved));
		manager.unReserve(3, 1);
		assertEquals(null, manager.reserveConsecutive(5));
		expected = 1;
		assertEquals(expected, manager.reserveConsecutive(4));
		assertEquals(intToString(new int[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }), boolToString(manager.reserved));

		manager.reserved = getBool(new int[] { 0 });
		assertEquals(intToString(new int[] { 0 }), boolToString(manager.reserved));

		assertEquals(null, manager.reserveConsecutive(2));
		expected = 0;
		assertEquals(expected, manager.reserveConsecutive(1));
		assertEquals(null, manager.reserveConsecutive(1));
		manager.unReserve(0, 1);
		expected = 0;
		assertEquals(expected, manager.reserveConsecutive(1));

	}
}
