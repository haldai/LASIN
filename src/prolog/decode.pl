:- ['test.pl'].

go1 :-
   test_decode('/home/daiwz/MATLAB/abduce/results/org_20/OS2/OS2_dict.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_20/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_20/OS2/OS2_dict.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/org_20/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_20/OS2/OS2_dict.csv',
               [20, 0.4, 0, 50],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_20/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_50/OS2/OS2_dict.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_50/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_50/OS2/OS2_dict.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/org_50/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_50/OS2/OS2_dict.csv',
               [50, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_50/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_100/OS2/OS2_dict.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_100/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_100/OS2/OS2_dict.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/org_100/OS2/MNIST_train_code.csv'),   
   test_decode('/home/daiwz/MATLAB/abduce/results/org_100/OS2/OS2_dict.csv',
               [100, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_100/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_200/OS2/OS2_dict.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_200/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_200/OS2/OS2_dict.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/org_200/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/org_200/OS2/OS2_dict.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/org_200/OS2/omniglot_test_code.csv').

gop :-
   test_decode('../../results/org_5/MP_dict.csv',
               [5, 0.4, 0, 50],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_5.csv'),
   test_decode('../../results/org_13/MP_dict.csv',
               [13, 0.4, 0, 50],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_13.csv'),
   test_decode('../../results/org_20/MP_dict.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_20.csv'),
   test_decode('../../results/org_25/MP_dict.csv',
               [25, 0.4, 0, 50],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_25.csv'),
   test_decode('../../results/org_50/MP_dict.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_50.csv'),
   test_decode('../../results/org_100/MP_dict.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_train_patches.csv',
               '../../data/MNIST_train_patches_code_100.csv'),
   test_decode('../../results/org_5/MP_dict.csv',
               [5, 0.4, 0, 50],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_5.csv'),
   test_decode('../../results/org_13/MP_dict.csv',
               [13, 0.4, 0, 50],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_13.csv'),
   test_decode('../../results/org_20/MP_dict.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_20.csv'),
   test_decode('../../results/org_25/MP_dict.csv',
               [25, 0.4, 0, 50],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_25.csv'),
   test_decode('../../results/org_50/MP_dict.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_50.csv'),
   test_decode('../../results/org_100/MP_dict.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_test_patches.csv',
               '../../data/MNIST_test_patches_code_100.csv').
   

go2:-   
   % strokes
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_20/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_50/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/OS2_dict_5.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/OS2_dict_5.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/OS2_dict_5.csv',
               [100, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_100/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/OS2_dict_1.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/OS2_dict_1.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/OS2_dict_1.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/stroke_200/OS2/omniglot_test_code.csv').
go3:-
   % kmeans
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_20/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_50/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/OS2_dict_1.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/OS2_dict_1.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/OS2_dict_1.csv',
               [100, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_100/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/kmeans_200/OS2/MNIST_test_code.csv').
go4:-   % spectral
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/OS2_dict_1.csv',
               [20, 0.4, 0, 50],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_20/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/OS2_dict_1.csv',
               [50, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_50/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/OS2_dict_15.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/OS2_dict_15.csv',
               [100, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/MNIST_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/OS2_dict_15.csv',
               [100, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_100/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/omniglot_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/omniglot_test_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_train.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/MNIST_train_code.csv'),
   test_decode('/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/OS2_dict_2.csv',
               [200, 0.4, 0, 100],
               '../../data/MNIST_test.csv',
               '/home/daiwz/MATLAB/abduce/results/spectral_200/OS2/MNIST_test_code.csv').
  


