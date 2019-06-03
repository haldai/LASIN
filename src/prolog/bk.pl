% background knowledge for sampling mnist
%========================================
:- use_module(library(random)).
:- discontiguous ink_/4.

recursion_limit(6).
neighbor_size(4).
angle_limit(1*pi/10).

% number 0 and 1 only have 1 component, other numbers have 2 components
num_components(T, 1):-
    T >= 0, T =< 199, !.
num_components(_, 2).

% stroke definition
stroke(_, [_]).
stroke(_, [_, _]).
stroke(Img, [P1, P2, P3]):-
    stroke(Img, P1, P2, P3).
stroke(Img, Points):-
    append(_, [P1, P2, P3], Points),
    stroke(Img, P1, P2, P3),
    append(Ps, [P3], Points),
    stroke(Img, Ps).
stroke(Img, P1, P2, P3):-
    angle_limit(T),
    angle(P1, P2, P3, Ang),
    Ang > T,
    ink(Img, P1, P2, P3).

get_strokes(Img, Points, Strokes):-
    get_strokes(Img, Points, [], Strokes, 0).
get_strokes(_, [], Re, Re, _).
get_strokes(_, Points, Re, Re, T):-
    length(Points, T).
get_strokes(Img, Points, Strokes, Re, _):-
    random_permutation(Points, Points_),
    greedy_discover_stroke(Img, Points_, Wasted, S),
    length(S, L),
    L > 2,
    remove_inlines(S, Wasted, Wasted1),
    get_strokes(Img, Wasted1, [S | Strokes], Re, 0),
    !.
get_strokes(Img, Points, Strokes, Re, T):-
    Points = [P | Ps],
    append(Ps, [P], Points2),
    T1 is T + 1,
    get_strokes(Img, Points2, Strokes, Re, T1),
    !.

greedy_discover_stroke(Img, Ps, Wasted, Points):-
    greedy_discover_stroke(Img, Ps, [], Wasted, [], Points).
greedy_discover_stroke(_, [], Wa, Wa, Re, Re).
greedy_discover_stroke(Img, [P | Ps], Wasted, Wa, Points, Re):-
    append(Points, [P], St),
    stroke(Img, St),
    % remove_surrounding(P, Ps, PPs),
    PPs = Ps,
    greedy_discover_stroke(Img, PPs, Wasted, Wa, St, Re),
    !.
greedy_discover_stroke(Img, [P | Ps], Wasted, Wa, Points, Re):-
    append([P], Points, St),
    stroke(Img, St),
    % remove_surrounding(P, Ps, PPs),
    PPs = Ps,
    greedy_discover_stroke(Img, PPs, Wasted, Wa, St, Re),
    !.
greedy_discover_stroke(Img, [P | Ps], Wasted, Wa, Points, Re):-
    greedy_discover_stroke(Img, Ps, [P | Wasted], Wa, Points, Re).


% first-order background knowledge
% angle of three points "AB and BC"
angle([X1, Y1], [X2, Y2], [X3, Y3], A):-
    dot([X2 - X1, Y2 - Y1], [X2 - X3, Y2 - Y3], P),
    eu_dist([X2 - X1, Y2 - Y1], [0, 0], D1),
    eu_dist([X2 - X3, Y2 - Y3], [0, 0], D2),
    Cos is round(P/(D1*D2)*10000)/10000, % 4 digits after '.'
    A is acos(Cos).

% dot/3: Inner product of two lists (vectors)
dot([], [], 0).
dot([X | Xs], [Y | Ys], Result):-
    Prod is X*Y,
    dot(Xs, Ys, Remaining),
    Result is Prod + Remaining.

% eu_dist/3: Euclidean distance between two vectors
eu_dist_sum([], [], 0).
eu_dist_sum([X | Xs], [Y | Ys], Sum):-
    Dist is (X - Y)^2,
    eu_dist_sum(Xs, Ys, Remaining),
    Sum is Dist + Remaining.
eu_dist(X, Y, Result):-
    eu_dist_sum(X, Y, Sum),
    Result is sqrt(Sum).

% determine wheter ink exists on image
ink(Img, P1, P2):-
    recursion_limit(N),
    ink_(Img, P1, P2, N).
ink(Img, P1, P2, P3):-
    recursion_limit(N),
    ink_(Img, P1, P2, N),
    ink_(Img, P2, P3, N).

ink_(Img, P1, P2, 0):-
    ink_(Img, P1),
    ink_(Img, P2),
    !.

point_equal([X1, Y1], [X2, Y2]):-
    X1 == X2, Y1 == Y2.

ink_(Img, P1, P2, T):-
    T > 0,
    midpoint(P1, P2, P3),
    not(point_equal(P1, P3)),
    not(point_equal(P2, P3)),
    T1 is T - 1,
    ink_(Img, P1, P3, T1),
    ink_(Img, P3, P2, T1).

ink_(Img, P1, P2, T):-
    T > 0,
    midpoint(P1, P2, P3),
    point_equal(P1, P3).

ink_(Img, P1, P2, T):-
    T > 0,
    midpoint(P1, P2, P3),
    point_equal(P2, P3).

midpoint([X1, Y1], [X2, Y2], [X3, Y3]):-
    X_d is (X1 + X2)/2,
    Y_d is (Y1 + Y2)/2,
    X3 is truncate(X_d + 0.5),
    Y3 is truncate(Y_d + 0.5),
    !.

remove_surrounding(P1, Ps, Rest):-
    remove_surrounding(P1, Ps, [], Rest).
remove_surrounding(_, [], Rest, Rest).
remove_surrounding(P1, [P2 | Ps], Rest, Re):-
    neighbor_size(S),
    eu_dist(P1, P2, D),
    D >= S,
    remove_surrounding(P1, Ps, [P2 | Rest], Re),
    !.
remove_surrounding(P1, [_ | Ps], Rest, Re):-
    remove_surrounding(P1, Ps, Rest, Re), !.

remove_surroundings([], Rest, Rest).
remove_surroundings([P | Ps], Points, Rest):-
    remove_surrounding(P, Points, Lefted),
    remove_surroundings(Ps, Lefted, Rest).

remove_inlines([_], Rest, Rest).
remove_inlines([P1, P2 | Ps], Wasted, Rest):-
    line_points(P1, P2, LP),
    remove_surroundings(LP, Wasted, Wasted1),
    remove_inlines([P2 | Ps], Wasted1, Rest).
