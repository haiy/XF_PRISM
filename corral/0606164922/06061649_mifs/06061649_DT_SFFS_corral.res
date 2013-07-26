
J48 pruned tree
------------------

index5 = 0: 1 (68.0/14.0)
index5 = 1
|   index0 = 0
|   |   index2 = 0: 1 (8.0)
|   |   index2 = 1: 0 (16.0/4.0)
|   index0 = 1: 0 (36.0/6.0)

Number of Leaves  : 	4

Size of the tree : 	7


Time taken to build model: 0.02 seconds
Time taken to test model on training data: 0.01 seconds

=== Error on training data ===

Correctly Classified Instances         104               81.25   %
Incorrectly Classified Instances        24               18.75   %
Kappa statistic                          0.616 
Mean absolute error                      0.2987
Root mean squared error                  0.3865
Relative absolute error                 60.6761 %
Root relative squared error             77.9042 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 42 14 |  a = 0
 10 62 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances         104               81.25   %
Incorrectly Classified Instances        24               18.75   %
Kappa statistic                          0.616 
Mean absolute error                      0.3067
Root mean squared error                  0.3977
Relative absolute error                 62.2648 %
Root relative squared error             80.127  %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 42 14 |  a = 0
 10 62 |  b = 1

