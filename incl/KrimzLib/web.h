#pragma once


namespace kl {
	class web {
	public:
		// Downloads website data(bytes) from the given url
		static bytes GetWebsiteData(std::string url) {
			bytes siteData;

			

			// Return data
			return siteData;
		}

		// Downloads data from the website and saves it in a file
		static void DownloadToFile(std::string url, std::string fileName) {
			file::WriteBytes(GetWebsiteData(url), fileName);
		}

		// Simple TCP server
		class TcpServer {
		public:
			~TcpServer() {
				Destroy();
			}

			// Returns server status
			bool IsCreated() {
				return created;
			}

			// Create a new server
			void Create(int port) {
				if (!created) {
					
					created = true;
				}
			}

			// Wait for a client to connect
			void WaitForClient() {
				if (!clientConnected) {
					
					clientConnected = true;
				}
			}

			// Return the currently connected clients name or empty string if no one is connected
			std::string Client() {
				if (created && clientConnected) {
					return clientName;
				}
				return "";
			}

			// Send data back to the client
			void SendData(bytes& data) {
				if (created && clientConnected) {
					
				}
			}
			void SendData(bytes&& data) {
				SendData(data);
			}

			// Receive data from the client
			void ReceiveData(bytes& dataBuffer) {
				if (created && clientConnected) {
					
				}
			}

			// Destroys the server
			void Destroy() {
				
				created = false;
			}

		private:
			bool clientConnected = false;
			bool created = false;
			std::string clientName = "";
		};

		// Simple TCP client
		class TcpClient {
		public:
			~TcpClient() {
				Disconnect();
			}

			// Returns the current state of the client
			bool IsConnected() {
				return connected;
			}

			// Connects the client to server
			void Connect(std::string serverIP, int serverPort) {
				// Disconnect the client if it's already connected
				Disconnect();


				
				connected = true;
			}

			// Send data to server
			void SendData(bytes& data) {
				if (connected) {
					
				}
			}
			void SendData(bytes&& data) {
				SendData(data);
			}

			// Receive data from the server
			void ReceiveData(bytes& dataBuffer) {
				if (connected) {
					
				}
			}

			// Dsiconnects the client
			void Disconnect() {
				if (connected) {
					
					connected = false;
				}
			}

		private:
			bool connected = false;
		};
	};
}
