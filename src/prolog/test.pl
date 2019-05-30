:- ['sampler.pl'],
   ['bk.pl'],
   load_foreign_library(foreign('../core/pl_data.so')),
   load_foreign_library(foreign('../core/pl_models.so')),
   load_foreign_library(foreign('../core/pl_cluster.so')),
   load_foreign_library(foreign('../core/pl_mnist.so')).

test(Atom, Weight1, Turn):-
    load_data('../../data/MNIST_100.csv', Data),
    write('Data loaded.'), nl,
    sparse_coder(Data, Model, [Atom, Weight1, 1, Turn], Codes),
    write('Training ended.'), nl,
    get_data_size(Codes, L),
    write('Coded data size: '), write(L), nl,
    select_data(Codes, [1, 3, 5, 7, 9, 101, 103, 105, 107, 109], Sel),
    print_data(Sel),
    write('Model: '), write(Model), nl,
    sc_save_dict(Model, 'dict.csv'),
    free_data(Data), free_data(Codes), free_data(Sel), free_sc(Model).

test_centering:-
    load_data('../../data/dev2k.csv', Data),
    %load_data('../../data/dev10.csv', Data),
    mnist_gen_masks(Data, _, 0.5, [], _, Mask),
    sc_mask_patches(Data, Mask, Patches_),
    centering_data(Patches_, Patches),
    %print_data(Patches, '/home/daiwz/MATLAB/abduce/patches.csv'),
    print_data(Patches, 'patches.csv'),
    %sparse_coder(Patches, M, [500, 0.4, 0, 20], _),
    sparse_coder(Patches, M, [50, 4, 0, 20], _),
    %sc_save_dict(M, '/home/daiwz/MATLAB/abduce/dict.csv').
    sc_save_dict(M, 'dict.csv').
test_mask:-
    load_data('../../data/dev2k.csv', Data),
    %load_data('../../data/dev10.csv', Data),
    mnist_gen_masks(Data, _, 0.5, [], _, Mask),
    sc_mask_patches(Data, Mask, Patches_),
    centering_data(Patches_, Patches),
    print_data(Patches, '/home/daiwz/MATLAB/abduce/patches.csv').
    %sparse_coder(Patches0, M1, [20, 0.7, 1, 100], _),
    %mnist_gen_masks(Data, M1, 0.5, C0, C1, Mask1),
    %sc_mask_patches(Data, Mask1, Patches1),
    %sparse_coder(Patches1, M2, [20, 0.7, 1, 100], _),
    %sc_save_dict(M2, '/home/daiwz/MATLAB/abduce/dict.csv').

test_go(Turn, Model, [Atom, L1, L2, SC_Turn]):-
    load_data('../../data/MNIST_1000.csv', Data),
    test_learn(Data, _, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model), !.


test_go_M1(Turn, Model, [Atom, L1, L2, SC_Turn], _):-
    load_data('../../data/MNIST_100.csv', Data),
    test_learn(Data, _, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model), !.

test_go_M(Turn, Model, [Atom, L1, L2, SC_Turn], _):-
    load_data('../../data/MNIST_train.csv', Data),
    test_learn(Data, _, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model), !.

test_go_os1(Turn, Model, [Atom, L1, L2, SC_Turn], _):-
    load_data('../../data/omniglot_small1_train.csv', Data),
    test_learn(Data, _, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model), !.

test_go_os2(Turn, Model, [Atom, L1, L2, SC_Turn], _):-
    load_data('../../data/omniglot_small2_train.csv', Data),
    test_learn(Data, _, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model), !.

test_learn(_, M, _, _, _, 0, Model_final):-
    Model_final = M,
    !.
test_learn(Data, M, Thresh, Param, Last_Centers, T, Model_final):-
    Param = [Atom, L1, L2, SC_Turn],
    mnist_gen_masks(Data, M, Thresh, Last_Centers, New_Centers, Mask),
    sc_mask_patches(Data, Mask, Patches),
    print_data(Patches, 'patches.csv'),
    sparse_coder(Patches, M1, [Atom, L1, L2, SC_Turn], _),
    % save dict for each turn
    %Fname = '/home/daiwz/MATLAB/abduce/dict',
    Fname = 'dict',
    %D1name = '/home/lamda/Storage_SV52/daiwz/abduce/results/',
    D1name = 'results/',
    %D2 = BG,
    Ftail = '.csv',
    name(Fname, NList), name(T, NT), name(Ftail, NTL),
    append(NT, NTL, NTT),
    append(NList, [95 | NTT], NNList), name(Nname, NNList),
    sc_save_dict(M1, Nname),
    T1 is T - 1,
    % current model and data are useless now
    free_sc(M), free_data(Patches),
    test_learn(Data, M1, Thresh, Param, New_Centers, T1, Model_final).

test_stroke:-
    load_data('../../data/MNIST_100.csv', Data),
    get_instance(Data, 0, Inst),
    sc_sample_mask_center(Data, Model, 0.5, 50, Bad_dims),
    write('sample mask OK!'), nl,
    mnist_dim2coord(Bad_dims, [P0 | _]),
    write('coordinate trans OK!'), nl,
    list_to_set(P0, PP0),
    ppl(PP0),
    get_strokes(Inst, PP0, Strokes),
    write(Strokes), nl,
    ppll(Strokes),
    free_vec(Inst),
    free_data(Data).

ppll([]).
ppll([L | Ls]):-
    ppl(L),
    ppll(Ls).
ppl(L):-
    write('['), print_point_list(L), write(']'), nl.
print_point_list([]).
print_point_list([[X, Y] | Ls]):-
    write(X), write(','), write(Y), write(';'),
    print_point_list(Ls).

test_decode(DictPath, Param, File_in, File_out):-
    sc_load_model(Param, DictPath, Model),
    sc_encode_file(Model, File_in, File_out),
    free_sc(Model).

test_org(M, Param):-
    load_data('../../data/MNIST_100.csv', Data),
    Param = [Atom, L1, L2, SC_Turn],
    sparse_coder(Data, M, [Atom, L1, L2, SC_Turn], _),
    Fname = '/home/daiwz/MATLAB/abduce/results/org_20/dict.csv',
    sc_save_dict(M, Fname),
    free_sc(M), free_data(Data).
