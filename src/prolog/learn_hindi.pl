:- ['test_ol.pl'].

test_go_dev(Turn, TR, Path, Model, [Atom, L1, L2, SC_Turn], BG):-
    load_data(Path, Data),
    test_learn(Data, M, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model, BG, TR),
    free_data(Data), !.

test_go_tamil(Turn, Model, [Atom, L1, L2, SC_Turn], BG):-
    TR = 'tamil1k',
    load_data('../../data/tamil1k.csv', Data),
    test_learn(Data, M, 0.5, [Atom, L1, L2, SC_Turn], [], Turn, Model, BG, TR),
    free_data(Data), !.

learn_dict(W):-
    % test_go_dev(10, 'dev2k', '../../data/dev2k.csv', Model, [200, W, 0, 50], stroke).
    test_go_dev(10, 'dev2k', '../../data/dev2k.csv', Model, [200, W, 0, 50], stroke).
    % test_go_tamil(10, Model, [200, 0.7, 0, 50], stroke).

learn_org:-
    TR = 'dev2k',
    load_data('../../data/dev2k.csv', Data),
    test_learn(Data, M, 0.5, [200, 0.4, 0, 50], [], 1, Model, org, TR),
    free_data(Data).
