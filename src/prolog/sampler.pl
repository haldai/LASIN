% abduce data patches from learned dictionary
%==============================================
:-use_module(library('lists')).
:-['utils.pl'].
% generate masks for sampling data patches
%    A mask is a set of indicators [0, 0, 1, 1, ...] which
%    indicates which dimension of feature is sampled. We
%    assume that each character is composed by two parts,
%    so Mask = [[Mask1], [Mask2]], and intsct(Mask1, Mask2) == [].
mnist_gen_masks(Data, Model, Thresh, Last_Centers, New_Centers, Masks):-
    % Randomly find dimensions that have large error.
    sc_sample_mask_center(Data, Model, Thresh, 50, Bad_dims),
    write('sample mask OK!'), nl,
    mnist_dim2coord(Bad_dims, Mask_centers),
    write('coordinate trans OK!'), nl,
    mnist_merge_mask_centers(Data, Mask_centers, Last_Centers, New_Centers, 0),
    write('mask merge OK!'), nl,
    mnist_create_masks(New_Centers, Masks).

% merge old mask centers with new centers
mnist_merge_mask_centers(_, [], [], [], _):-
    !.
mnist_merge_mask_centers(Data, [C | Centers], [], [NC | New_Centers], T):-
    write('merging mask '), write(T), nl,
    mnist_merge_mask(Data, C, [], NC, T),
    T1 is T + 1,
    mnist_merge_mask_centers(Data, Centers, [], New_Centers, T1),
    !.
mnist_merge_mask_centers(Data, [C | Centers], [OC | Old_Centers], [NC | New_Centers], T):-
    write('merging mask '), write(T), nl,
    mnist_merge_mask(Data, C, OC, NC, T),
    T1 is T + 1,
    mnist_merge_mask_centers(Data, Centers, Old_Centers, New_Centers, T1).

% merge a center with old centers
% mnist_merge_mask(C, [], M):-
%     M = [C, []],
%     !.
%mnist_merge_mask(C, [M1 | [[]]], M):-
%    append(M1, C, M2),
%    kmeans(M2, 2, M),
%    !.
mnist_merge_mask(Data, C, M0, M, T):-
    append_lists(M0, M1),
    append(M1, C, M2),
    list_to_set(M2, M3),
    get_instance(Data, T, Inst),
    get_strokes(Inst, M3, Strokes),
    strokes_mask(Strokes, [], M),
    free_vec(Inst).
    % num_components(T, NC),
    % spectral_cluster(M3, 2, 3, NC, M).
    % kmeans(M3, NC, M).

mnist_create_masks([], []):-
    !.
mnist_create_masks([F | Fs], [M | Ms]):-
    mnist_create_mask_grps(F, M),
    mnist_create_masks(Fs, Ms).

mnist_create_mask_grps([], []):-
    !.
mnist_create_mask_grps([G | Gs], [M | Ms]):-
    mnist_create_mask(G, 1, M),
    mnist_create_mask_grps(Gs, Ms).

% from 1 dimensional position to 2 dimensional coordinates
mnist_dim2coord([], []).
mnist_dim2coord([P | Ps], [C | Cs]):-
    mnist_dim2coord_(P, C),
    mnist_dim2coord(Ps, Cs).

mnist_dim2coord_([], []).
mnist_dim2coord_([D | Ds], [C | Cs]):-
    X is D mod 28,
    Y is D div 28,
    C = [X, Y],
    mnist_dim2coord_(Ds, Cs).

% stroke to mask
strokes_mask([], Re, Re).
strokes_mask([S | Ss], Temp, Masks):-
    stroke_points(S, [], M),
    length(M, LM),
    (LM >= 8 ->
         (append(Temp, [M], Temp1),
          strokes_mask(Ss, Temp1, Masks), !);
     (strokes_mask(Ss, Temp, Masks), !)
     ). 

stroke_points([P], Temp, Re):-
    append(Temp, [P], Re).
stroke_points([P1, P2 | Ps], Temp, Re):-
    line_points(P1, P2, P),
    append(Temp, P, Temp1),
    stroke_points([P2 | Ps], Temp1, Re).
