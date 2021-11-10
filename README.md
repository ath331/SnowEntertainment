# SnowEntertainment

SnowEntertainment 

패킷 설계?

TcpSession ( 유저마다 들고 있음 ) 에서 적절한 사이즈의 데이터를 받으면?

PacketProject의 패킷관리 클래스 ( 이름은 아마 패킷 디스패쳐? )로 데이터를 넘기고

패킷 디스패쳐에서 GameProject의 있는 적절한 핸들러를 찾아서 해당 핸들러로 로직을 태우기.