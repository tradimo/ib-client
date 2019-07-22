class CClient {
public:
	CClient() = default;
	
	bool connect();
	bool isConnected() const { return true; }
};
