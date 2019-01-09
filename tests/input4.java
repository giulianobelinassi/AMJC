class Factorial {
    public static void main(String[] a) {
        System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {
    int aux;
    public int ComputeFac(int num) {
        int i;
        i=1;
        aux=1;
        while (i <= num) {
            aux = aux * i;
            i = i + 1;
        }

        return aux;
    }
}
