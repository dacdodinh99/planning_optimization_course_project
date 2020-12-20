package code;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

import org.chocosolver.solver.Model;
import org.chocosolver.solver.Solution;
import org.chocosolver.solver.variables.IntVar;



public class Schedule {
	int N = 100;
	int[][] d = new int [N][N];
	public void input(String filepath) throws FileNotFoundException {
		Scanner scanner = new Scanner(new File(filepath));
		int number = scanner.nextInt();
		N = number;
		
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				number = scanner.nextInt();
				d[i][j] = number;
			}
		}
		
	}
	
	public void solve() {
		Model model = new Model("schedule");
		
		// Define sum and upperbound of sum
		IntVar sum;
		int upperbound = 0;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				upperbound += d[i][j];
			}
		}
		upperbound *= (2 * N - 1);
		sum = model.intVar("sum", 0, upperbound);
		
        
		// T: de tao thanh constrain moi vong moi doi chi thi dau o mot tran
		IntVar[][] t = new IntVar[2 * N - 2][N];
		
		for(int round = 0; round < 2 * N - 2; round++) {
			for(int center = 0; center < N; center++) {
				t[round][center] = model.intVar("t" + round + "_" + center, 1, 1);
			}
		}
		
		// c: de tao thanh constrain cac tran khong lap lai o cac vong khac nhau
		IntVar[][] c = new IntVar[N][N];
				
		for(int home = 0; home < N; home++) {
			for(int guest = 0; guest < N; guest++) {
				if(home != guest) {
					c[home][guest] = model.intVar("t" + home + "_" + guest, 1, 1);
				}
				else {
					c[home][guest] = model.intVar("t" + home + "_" + guest, 0, 0);
				}
			}
		}
		
		IntVar[][][] X = new IntVar[2 * N - 2][N][N];
        for(int round = 0; round < 2 * N - 2; round++) {
        	for(int home = 0; home < N; home++) {
        		for(int guest = 0; guest < N; guest++) {
        			if(guest != home) {
        				X[round][home][guest] = model.intVar("X[" + round + "][" + home + "][" + guest + "]", 0, 1);
        			}
        			else {
        				X[round][home][guest] = model.intVar("X[" + round + "][" + home + "][" + guest + "]", 0, 0);
        			}
        		}
        	}
        }
        

        for(int round = 0; round < 2 * N - 2; round++) {
        	for(int center = 0; center < N; center++) {
        		for(int neighbor = 0; neighbor < N; neighbor++) {
        			model.arithm(t[round][center], "=", t[round][center], "+", X[round][center][neighbor]);
        			model.arithm(t[round][center], "=", t[round][center], "+", X[round][neighbor][center]);
        	    }
        	}
        }
        
        
    	for(int home = 0; home < N; home++) {
    		for(int guest = 0; guest < N; guest++) {
    			for(int round = 0; round < 2 * N - 2; round++) {
    				model.arithm(c[home][guest], "=" , c[home][guest], "+", X[round][home][guest]);
    			}
    			
    		}
    	}
    	
        // find minimum solution
//        Solution solution = model.getSolver().findOptimalSolution(sum, false);
        int opti = upperbound;
        for(int i = 1; i <= 2; i++) {
        	Solution solution = model.getSolver().findSolution();
            if(solution != null){
         		System.out.println("Do dai toi uu o lan tim thu " + i);
         		System.out.println(solution.getIntVal(sum));
         		for(int round = 0; round < 2 * N - 2; round++) {
                	for(int home = 0; home < N; home++) {
                		for(int guest = 0; guest < N; guest++) {
                			System.out.println("X[" + (round + 1) + "][" + (home + 1)+ "][" + (guest + 1) + "]" + solution.getIntVal(X[round][guest][home]));
                		}
                	}
         		}
         		
         		if(opti > solution.getIntVal(sum)) {
         			opti = solution.getIntVal(sum);
         		}
             }
             else {
             	System.out.println("No solution");
             	break;
             }
        }
        System.out.print("optimal finding solution: " + opti);
             

	}
	public static void main(String[] args) throws FileNotFoundException {
		Schedule app = new Schedule();
		app.input("data_6.txt");
        app.solve();
	}
}
