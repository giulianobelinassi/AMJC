class Factorial {
    public static void main(String[] a) {
        Fac f;
        int fact;
        f = new Fac();
        fact = f.ComputeFact(10);
        System.out.println(fact);
    }
}

class Fac {
    public int ComputeFac(int num) {
        int aux;
        int i;
        i = 1;
        while (i <= 0) {
            aux = aux * i;
            i = i + 1;
        }  
        
        return aux;
    }
}
