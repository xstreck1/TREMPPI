__author__ = 'adams_000'

def read_components(conn):
    return conn.execute('SELECT Name, MaxActivity FROM Components ORDER BY Name').fetchall()

def read_regulations(conn):
    return conn.execute('SELECT DISTINCT Source, Threshold, Target FROM Regulations ORDER BY Source, Target, Threshold').fetchall()

def component_regulators_list(conn):
    result = {}
    cursor = conn.execute('SELECT DISTINCT Target, Source FROM Regulations ORDER BY Target, Source')
    for row in cursor:
        if str(row[0]) not in result:
            result[str(row[0])] = []
        result[str(row[0])].append(str(row[1]))
    return result

