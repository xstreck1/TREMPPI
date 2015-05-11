__author__ = 'adams_000'

def read_components(conn):
    result = {}
    cursor = conn.execute('SELECT Name, MaxActivity FROM Components ORDER BY Name')
    for row in cursor:
        result[str(row[0])] = int(row[1])
    return result

def read_regulations(conn):
    result = {}
    cursor = conn.execute('SELECT DISTINCT Source, Target FROM Regulations ORDER BY Source')
    for row in cursor:
        if str(row[0]) not in result:
            result[str(row[0])] = []
        result[str(row[0])].append(str(row[1]))
    return result
