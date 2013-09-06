
J48 pruned tree
------------------

index0 = 0
|   index2 = 0: 1 (32.0)
|   index2 = 1
|   |   index3 = 0: 1 (16.0)
|   |   index3 = 1: 0 (16.0)
index0 = 1
|   index1 = 0
|   |   index2 = 0: 1 (16.0)
|   |   index2 = 1
|   |   |   index3 = 0: 1 (8.0)
|   |   |   index3 = 1: 0 (8.0)
|   index1 = 1: 0 (32.0)

Number of Leaves  : 	7

Size of the tree : 	13


Time taken to build model: 0.03 seconds
Time taken to test model on training data: 0.01 seconds

=== Error on training data ===

Correctly Classified Instances         128              100      %
Incorrectly Classified Instances         0                0      %
Kappa statistic                          1     
Mean absolute error                      0     
Root mean squared error                  0     
Relative absolute error                  0      %
Root relative squared error              0      %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 56  0 |  a = 0
  0 72 |  b = 1



=== Stratified cross-validation ===

Correctly Classified Instances         121               94.5313 %
Incorrectly Classified Instances         7                5.4688 %
Kappa statistic                          0.8878
Mean absolute error                      0.0549
Root mean squared error                  0.2183
Relative absolute error                 11.1539 %
Root relative squared error             43.9778 %
Total Number of Instances              128     


=== Confusion Matrix ===

  a  b   <-- classified as
 50  6 |  a = 0
  1 71 |  b = 1

