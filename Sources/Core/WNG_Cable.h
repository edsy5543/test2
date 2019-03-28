// +----------------------------------------------------------------------------------------------------------+
// !                  Application WANDA NG : Gestion du cablage                                                !
// !  - Classe WNG_Cable : Gestion des cable du projet                                        !
// +----------------------------------------------------------------------------------------------------------+
#ifndef WNG_CABLE_H
#define WNG_CABLE_H

#include "WNG_Defs.h"
#include "WNG_Util.h"
#include "WNG_CablingCatalog.h"

namespace WNG {
	// +-----------------------------------------------------------------------------------------+
	// !  - Classe Cable_Vertex :                                           !
	// +-----------------------------------------------------------------------------------------+
	class Cable_Vertex
	{
	public: //Constructor
		Cable_Vertex() {}
		Cable_Vertex(const Cable_Vertex& rh){ this->operator= (rh); }
		Cable_Vertex(const int floor_id1, const int floor_id2, const Point _Vertex);

		Cable_Vertex & operator= (const Cable_Vertex &rh);

		bool operator== (Cable_Vertex const& rh) {
			return (this->_PairFloor == rh._PairFloor && this->_Vertex == rh._Vertex);
		}
		void Init(const int floor_id1, const int floor_id2, const Point& _Vertex){
			this->_PairFloor = std::make_pair(floor_id1, floor_id2);
			this->_Vertex = _Vertex;
		}
		void Save(std::ofstream& o) const;
		void Load(std::ifstream& i);
		void Move(const double dx, const double dy) { _Vertex.Move(dx, dy);}

	public: // Public assesseurs
		Point GetPoint() const{ return _Vertex; };
		double GetX() const { return _Vertex.GetX(); };
		double GetY() const { return _Vertex.GetY(); };
		void SetPoint(const Point vertex) { _Vertex = vertex; };
		void SetPoint(const double x, const double y) { _Vertex.Init(x, y); }
		std::pair<int, int> GetPairFloor() const { return _PairFloor;}
		int GetFloor(const bool b_first) const { return b_first ? _PairFloor.first : _PairFloor.second; }
		void SetFloor(const bool b_first, const int floor_id) { b_first ? _PairFloor.first = floor_id : _PairFloor.second = floor_id; }

		//int GetFloorId()
		////Initialisation d'une extremités du cable (b_first = true -> premiere extremité)
		void SetCablePort(const bool b_first, const int floor_id, const int device_id, const WNG_DasPort& device_port);

//	private: //Private attributs
		Point _Vertex;
		std::pair<int, int> _PairFloor;
	};

	// +-----------------------------------------------------------------------------------------+
	// !  - Classe WNG_CablePort : Extremité d'un cable                                           !
	// +-----------------------------------------------------------------------------------------+
	class WNG_CablePort
	{
	public: //Constructor
		WNG_CablePort() :_FloorId(UNDEFINED_VALUE), _DeviceId(UNDEFINED_VALUE){}
		WNG_CablePort(const WNG_CablePort& rh){ this->operator= (rh); }
		WNG_CablePort(const int floor_id, const int device_id, const WNG_DasPort device_port);

		WNG_CablePort & operator= (const WNG_CablePort &rh);

		bool operator== (WNG_CablePort const& rh) {
			return (this->_FloorId == rh._FloorId && this->_DeviceId == rh._DeviceId &&
				this->_FloorCx == rh._FloorCx && this->_DevicePort == rh._DevicePort);
		}

	public: // Public methods
		void Init(const int floor_id, const int device_id, const WNG_DasPort& device_port){
			_FloorId = floor_id; _DeviceId = device_id; _DevicePort = device_port;
		}
		void Save(std::ofstream& o) const;
		void Load(std::ifstream& i);

	public: //Public attributs
		int _FloorId;
		int _DeviceId;
		WNG_DasPort _DevicePort;
		Point _FloorCx;

	private: //Private attributs

	};

	// +-----------------------------------------------------------------------------------------+
	// !  - Classe WNG_Cable : Gestion des cable du projet                                        !
	// +-----------------------------------------------------------------------------------------+
	class WNG_Cable
	{
	public: //Constructor
		WNG_Cable(){};
		WNG_Cable(const WNG_Cable& rh){ this->operator= (rh); }

		WNG_Cable& operator= (const WNG_Cable& rh);
		bool operator== (WNG_Cable const& rh);

	public: // Public methods
		void Save(std::ofstream& o) const;
		void Load(std::ifstream& i);

		//Initialisation du cable
		void Init(const int id, const WNG_DasParam& dasParam, const std::vector<Cable_Vertex>& list_vertex, const std::pair<WNG_CablePort, WNG_CablePort>& pairePort);

		//Initialisation d'une extremités du cable (b_first = true -> premiere extremité)
		void SetCablePort(const bool b_first, const int floor_id, const int device_id, const WNG_DasPort& device_port);
		
		//Move de tous les vertex suivant dx, dy
		void Move(const double dx, const double dy);
		
		//Insert un vertex après le vertex d'indice position 
		void InsertVertex(const int position, const Cable_Vertex& vertex);
		//Modifie le vertex d'indice position 
		void ModifyVertex(const int position, const Cable_Vertex& vertex);
		//Delete le vertex d'indice position 
		void DeleteVertex(const int position);
		//Retourne indice du vertex proche de x,y, -1 si >0.05
		int GetVertexIndice(const double x, const double y) const;
		//Retourne la liste des vertex du cable présent sur l'étage floor_id 
		std::vector<Cable_Vertex> GetListVertexFloor(const int floor_id) const;

		//Retourne le WNG_CablePort de l'extrémité du cable(b_first = true -> premiere extremité)
		WNG_CablePort GetCablePort(const bool b_first) const;
		//Retourne l'id du device de l'extrémité du cable (b_first = true -> premiere extremité)
		int GetDeviceId(const bool b_first) const;
		//Retourne le WNG_DasPort du device de l'extrémité du cable (b_first = true -> premiere extremité)
		WNG_DasPort GetDevicePort(const bool b_first) const;
		//Retourne le TypeName du device de l'extrémité du cable (b_first = true -> premiere extremité)
		std::string GetDevicePortTypeName(const bool b_first) const;

		//Setting _PairPort et _PairFloor de tous les vertex avec valeur de floor_id
		void SetFloor(const int floor_id);
		//Setting le WNG_DasPort du device de l'extrémité du cable (b_first = true -> premiere extremité)
		void SetDevicePort(const bool b_first, const WNG_DasPort& dasPort);


	public: // Public assesseurs
		void reinit() {
			_List_vertex.clear();
			_PairPort.first = WNG_CablePort();
			_PairPort.second = WNG_CablePort();
		}
		int GetId() const{ return _Id; };
		void SetId(const int id) { _Id = id; };
		std::pair<WNG_CablePort, WNG_CablePort>& GetPorts() { return _PairPort; }
		WNG_CablePort& GetPort(const bool b_first) { return b_first ? _PairPort.first : _PairPort.second; }
		void SetDasParam(const WNG_DasParam& DasParam)  { _DasParam = DasParam; }
		std::vector<Cable_Vertex>* GetListVertex() { return &_List_vertex; }
		//const std::vector<Cable_Vertex> GetListVertex() { return _List_vertex; }
		std::string GetReference() const { return _DasParam.GetReference(); }
		std::string GetSupplier() const { return _DasParam.GetSupplier(); }
		CABLING::CLASS_DAS GetClass() const { return _DasParam.GetClass(); }

	private: //Private attributs
		int _Id;
		WNG_DasParam _DasParam;	//Parametres du cable
	public: //public attributs
		std::pair<WNG_CablePort, WNG_CablePort> _PairPort; // <Port 1er vertex, Port dernier vertex>
	public: //public attributs
		std::vector<Cable_Vertex> _List_vertex; //Liste des Points (Cable_Vertex)
	};
}
#endif // WNG_CABLE_H
