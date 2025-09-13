import numpy as np
import sys,argparse
from scipy.special import roots_legendre, eval_legendre
import csv 

if __name__ == "__main__": 
    
    # get the max number of points to print
    max_order = 10
    if len(sys.argv) > 1: 
        max_order = int(sys.argv[1])
     
    name = "gauss_quad_points_" + str(max_order) + ".dat"

    # now, write the outputs for each line
    with open(name, 'w', newline='') as f:

        writer = csv.writer(f, delimiter=' ')

        # get the roots for each order
        for order in range(1, max_order+1): 
            
            # get the roots of this order
            roots, weights = roots_legendre(order)

            writer.writerow(["#-order", order])

            # now, print the result
            for row in range(0, len(roots)): 
                
                writer.writerow([roots[row], weights[row]])
            
            writer.writerow(["#"])





    


