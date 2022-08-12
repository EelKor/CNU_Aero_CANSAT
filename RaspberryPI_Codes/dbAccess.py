import pymysql

class DBAccess():

    def __init__(self):

        # __db_login은 보안을 위해 privite
        self.__db_login = pymysql.connect(
             user='canset',
             passwd='canset',
             host='localhost',
             port=3306,
             db='cansetLog',
             charset='utf8'


        )
        # pymysql 을 사용하기 위한 기본 설정
        self.cursor = self.__db_login.cursor(pymysql.cursors.DictCursor)

    ##### 메소드 updateall 설명 ######
    # 데이터베이스를 업데이트 하는 것으로 데이터베이스내 모든 데이터를 최신화
    def upload(self, insert_data):
        self.__sql = "INSERT INTO log(dt, acc_x, acc_y, acc_z, gyo_x, gyo_y, gyo_z, pressure, alt, altGPS, temp, speed, roll, pitch, lat, lng) " \
                     "VALUES(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"



        self.cursor.execute(self.__sql, insert_data)
        self.__db_login.commit()

    def upload_notice(self, insert_data):
        self.__sql = "INSERT INTO notice(content, whos) VALUES(%s, %s);"
        self.cursor.execute(self.__sql, insert_data)
        self.__db_login.commit()

    def read(self):
        __readsql = "SELECT * FROM log"
        self.cursor.execute(__readsql)
        return self.cursor.fetchall()

    def readone(self):
        __readsql = "SELECT * FROM log ORDER BY no DESC limit 1;"
        self.cursor.execute(__readsql)
        return self.cursor.fetchall()
	
    def readSOS(self):
        __readsql = "SELECT DISTINCT ip FROM RescueMsg;"
        self.cursor.execute(__readsql)
        return self.cursor.fetchall()

    def readNotice(self):
       __readsql = "SELECT * FROM notice;"
       self.cursor.execute(__readsql)
       return self.cursor.fetchall()

    def close(self):
        self.cursor.close()
