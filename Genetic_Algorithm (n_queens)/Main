
import java.util.Scanner;

public class Main
{
	public static int scanner1;//number of queens
	
	public static void main(String[] args)
    {
    	Scanner scannerInteger =new Scanner(System.in);//input the sequence
    	do{
		System.out.println("Give me number of queens :");
		System.out.print(">>");
		scanner1=scannerInteger.nextInt();
		if(scanner1<=3) {
			System.out.println("\n***  The number must be greater than 3   ***\n");
		}
    	}while(scanner1>=3);
		scannerInteger.close();
		
		long startTime =System.currentTimeMillis();
        GeneticAlgorithm algorithm = new GeneticAlgorithm(scanner1);
        System.out.println("Iterations starting...");
        int sum=0;
        for(int i=1;i<scanner1;i++) {
        	sum+=i;
        }
        System.out.println();
        System.out.println("Fitness should be :"+sum);
        Chromosome solution = algorithm.run(200, 0.12,sum,scanner1);
        Chromosome.queen_num=scanner1;
        solution.print();
        long endTime =System.currentTimeMillis();
        System.out.println("Execution Time: " + (endTime-startTime)/1000F + "sec.");
       
    }
}
