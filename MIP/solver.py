from ortools.linear_solver import pywraplp

def gen_multidimensional_list(dimensions):
    assert(len(dimensions) > 0)
    if len(dimensions) > 1:
        return [gen_multidimensional_list(dimensions[1:]) for _ in range(dimensions[0])]
    else:
        return [0 for _ in range(dimensions[0])]

def read_data(data_path):
    f = open(data_path)
    n = int(f.readline())
    c = [[] for _ in range(n)]

    for i in range(n):
        c[i] = [int(x) for x in f.readline().split()]

    return (n, c)

def main():
    n, c = read_data('../data/data_6')

    # Create the mip solver with the SCIP backend.
    solver = pywraplp.Solver.CreateSolver('SCIP')

    X = gen_multidimensional_list([2 * n, n, n, n])

    for slot in range(2 * n):
        for team in range(n):
            for fr in range(n):
                for to in range(n):
                    X[slot][team][fr][to] = solver.IntVar(0.0, 1.0, f'X[{slot}][{team}][{fr}][{to}]')

    print('Number of variables =', solver.NumVariables())

    # Teams have to start at their homes
    # X[0][i][i][i] = 1
    for i in range(n):
        solver.Add(X[0][i][i][i] == 1)


    # Each slot, each team only move once
    for slot in range(0, 2 * n):
        for team in range(n):
            constraint = solver.RowConstraint(1, 1, '')
            for i in range(n):
                for j in range(n):
                    constraint.SetCoefficient(X[slot][team][i][j], 1)

    # Satisfying each round's condition
    for slot in range(1, 2 * n - 1):
        for to in range(n):
            constraint = solver.RowConstraint(0, 0, '')
            for i in range(n):
                constraint.SetCoefficient(X[slot][to][i][to], 1)
                if i == to:
                    continue
                for j in range(n):
                    constraint.SetCoefficient(X[slot][i][j][to], - 1)


    # Each team in each slot will move from a city to another city
    for slot in range(0, 2 * n - 1):
        for city in range(n):
            for team in range(n):
                constraint = solver.RowConstraint(0, 0, '')
                for i in range(n):
                    constraint.SetCoefficient(X[slot][team][i][city], 1)
                    constraint.SetCoefficient(X[slot + 1][team][city][i], -1)

    for u in range(n):
        for v in range(n):
            if u == v:
                continue
            constraint = solver.RowConstraint(1, 1, '')
            for slot in range(1, 2 * n - 1):
                for i in range(n):
                    constraint.SetCoefficient(X[slot][u][i][v], 1)

    for u in range(n):
        constraint = solver.RowConstraint(1, 1, '')
        for i in range(n):
            constraint.SetCoefficient(X[2 * n - 1][u][i][u], 1)

    print('Number of constraints =', solver.NumConstraints())

    objective = solver.Objective()
    for slot in range(2 * n):
        for team in range(n):
            for fr in range(n):
                for to in range(n):
                    objective.SetCoefficient(X[slot][team][fr][to], c[fr][to])
    objective.SetMinimization()

    status = solver.Solve()

    # print(solver.Objective().Value())


    if status == pywraplp.Solver.OPTIMAL:
        print('Objective value =', solver.Objective().Value())

        for slot in range(2 * n):
            for team in range(n):
                for i in range(n):
                    for j in range(n):
                        if X[slot][team][i][j].solution_value() == 1:
                            print(f'In round {slot}, team {team} move from {i} to {j}')
        
        for round in range(1, 2 * n - 1):
            cur = [0 for i in range(n)]
            for team in range(n):
                for i in range(n):
                    for j in range(n):
                        if X[round][team][i][j].solution_value() == 1:
                            cur[team] = j
            opp = [0 for i in range(n)]
            for team in range(n):
                if team != cur[team]:
                    print(f'({cur[team]}, {team})', end=' ')
            
            print()
            
            

        print()
        print('Problem solved in %f milliseconds' % solver.wall_time())
        print('Problem solved in %d iterations' % solver.iterations())
        print('Problem solved in %d branch-and-bound nodes' % solver.nodes())
    else:
        print('The problem does not have an optimal solution.')


if __name__ == '__main__':
    main()
