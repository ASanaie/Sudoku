
/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
struct scandid {
    int fixed[9][9];
    int choices[9][9];
};

int
main() {
    void print_table(struct scandid);
    int chk_row_col_subs(struct scandid);
    int matanalysis(struct scandid);
    int i, j, k;
    int resultchk = 0;
    struct scandid s1 = {.fixed = { // numbers= fixed   0=variable
            	{0,9,0,1,0,7,0,0,0},
            	{0,1,0,0,9,0,0,4,5},
            	{0,0,3,2,0,0,0,0,0},
            	{3,0,0,0,0,0,0,6,0},
            	{0,6,2,0,0,0,3,0,0},
            	{0,0,5,0,0,0,0,0,9},
            	{0,0,0,6,0,2,0,0,0},
            	{0,0,0,0,0,0,0,0,0},
            	{0,4,9,0,0,0,0,5,7}


        }
    };
    // let's keep track of single and multiples
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++) {
            if (1 <= s1.fixed[i][j] && s1.fixed[i][j] <= 9) {
                s1.choices[i][j] = 's'; //multiple
            } else {
                s1.choices[i][j] = 'm'; //single
            }
        }
    printf("Original Sudoku to solve\n");
    print_table(s1);
    matanalysis(s1);
    return 0;
}
int
chk_row_col_subs(struct scandid s) {
    int i, j, k;
    int r_check=0, c_check=0, i_check=0;
    int counter;
    counter = 0;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            for (k = j; k < 9; k++) {
                if (k != j) {
                    counter++;
                    r_check = s.fixed[i][j] == s.fixed[i][k]; // check horizontal duplicates at [i][j]
                    c_check = s.fixed[j][i] == s.fixed[k][i]; // check vertical values at [i][j]
                    //check inner 3x3 squares
                    i_check = s.fixed[3 * (i % 3) + (j % 3)][3 * (i / 3) + (j / 3)] == s.fixed[3 * (i % 3) + (k % 3)][3 * (i / 3) + (k / 3)];
                }
                if ((r_check || c_check || i_check ) && k != j ) {
                    return -1;
                }
            }
        }
    }
    return 0;
}


void
print_table(struct scandid s) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (s.choices[i][j]=='m') printf(" [] ");
            else if (s.choices[i][j]=='s') printf(" %1d  ", s.fixed[i][j]);
            else if (s.choices[i][j]=='f') printf(" x  ", s.fixed[i][j]);
        }
        printf("\n");
    }

}

int add_matrix_to_solution(struct scandid s0) {
    static struct scandid matrix_solution[2000];
    static sol_index=0;
    int i,j;
    int record;
    int flag_compare_bw_elements;
    int flag_compare_bw_records;
    int solution_is_new='y'; // assume this is new solution

    if(sol_index==0) {
        matrix_solution[sol_index]=s0;
        printf("from print function \ni=%d\n",sol_index);
        print_table(matrix_solution[sol_index]);
        sol_index++;
    }
    //compare with previous records
    flag_compare_bw_elements=1;
    flag_compare_bw_records=0;
    if (sol_index >=1 ) {
        for(record=0; record<sol_index; record++) {
            for(i=0; i<9; i++) {
                for(j=0; j<9; j++) {
                    // force units_the_same to zero if there is a difference
                    flag_compare_bw_elements == flag_compare_bw_elements && (matrix_solution[record].fixed[i][j]==s0.fixed[i][j]);
                }
            }
            flag_compare_bw_records=flag_compare_bw_records || flag_compare_bw_elements;
        }
    }
    if (flag_compare_bw_records==1) solution_is_new='n';
    // if a new solution is found
    if(solution_is_new == 'y' ) {
        sol_index++;
        matrix_solution[sol_index]=s0;
        printf("from print function \ni=%d\n",sol_index);
        print_table(matrix_solution[sol_index]);
    }
}

int
matanalysis(struct scandid s0) {
    int i, j, k, k_guess;
    int there_is_vari_cells=0;
    int guess_valid='y';
    int c;

    //check if previous failure or if all variable cells are taken
    for(i=0; i<9; i++) {
        for(j = 0; j < 9; j++) {
            //if(s0.choices[i][j] == 'f') {printf("exiting sub.....with Failure\n\n"); return -1;}
            if(s0.choices[i][j] == 'm') there_is_vari_cells=1;
        }
    }

    c = chk_row_col_subs(s0);

    if ( !there_is_vari_cells && c!=0) {
        printf("no variable cells . exiting recursive sub \n");
        return;
    }

    if (!there_is_vari_cells && c == 0) {
        //printf("solution found :\n\n\n");
        add_matrix_to_solution(s0);
    }
    // check if variable cells : no=we are at the end of tree
    //print_table(s0); printf("\n");
    //find next variable cells
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            //start branching & guessing if the cell is multiple 'm'
            if (s0.choices[i][j] == 'm') {
                //printf("variant at(%d,%d) \n", i,j);
                for (k_guess = 1; k_guess < 10  ; k_guess++) {
                    //limit forking by changing choice to single
                    guess_valid='y';
                    for(k=0; k<9 ; k++) {
                        if (s0.fixed[i][k]==k_guess && s0.choices[i][k]=='s') guess_valid='n';
                        if (s0.fixed[k][j]==k_guess && s0.choices[k][j]=='s') guess_valid='n';
                        if (s0.fixed[3 * (i / 3) + (k % 3)][3 * (j / 3) + (k / 3)]== k_guess)  guess_valid='n' ;
                    }
                    if(guess_valid=='y') {
                        s0.choices[i][j] = 's';
                        s0.fixed[i][j] = k_guess;
                        //printf(" (%d,%d)~=%d to rabbit hole ...",i,j, k_guess);
                        matanalysis(s0);
                    } //else printf("(%d,%d)!=%d  ",i,j, k_guess);
                    if(k_guess==9 && guess_valid=='n') {
                        s0.choices[i][j] = 'f'; // failure flag, exit on recall
                        //printf("<----out of the rabbit hole\n\n");
                        return;
                    }
                }
            }
        }
    }
}
