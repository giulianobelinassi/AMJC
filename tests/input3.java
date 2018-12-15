class Factorial {
    public static void main(String[] a) {
        System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {
    public int ComputeFac(int num) {
        int i;
        int[] A;
        A = new int[10];
        i = 1;
        A[0] = 1;
        while (i <= num) {
            A[i] = A[i-1] * i;
            i = i + 1;
        }

        return A[9];
    }
}
