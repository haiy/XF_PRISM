
J48 pruned tree
------------------

index5 = 0: 1 (68.0/14.0)
index5 = 1: 0 (60.0/18.0)

Number of Leaves  : 	2

Size of the tree : 	3


Time taken to build model: 0.01 seconds
Time taken to test model on training data: 0.01 seconds

=== Error on training data ===

Correctly Classified Instances          96               75      %
Incorrectly Classified Instances        32               25      %
Kappa statistic                          0.4961
Mean absolute error                      0.3706
Root mean squared error                  0.4305
Relative absolute error                 75.2757 %
Root relative squared error             86.772  %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 42 14 |  a = 0
 18 54 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances          94               73.4375 %
Incorrectly Classified Instances        34               26.5625 %
Kappa statistic                          0.4625
Mean absolute error                      0.3732
Root mean squared error                  0.4373
Relative absolute error                 75.7785 %
Root relative squared error             88.1083 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 40 16 |  a = 0
 18 54 |  b = 1

