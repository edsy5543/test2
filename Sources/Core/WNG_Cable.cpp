grggqrgq
#include "WNG_Device.h"
#include "WNG_Cable.h"

//********************************* Cable_Vertex **************************************************************

// +-----------------------------------------------------------------------------------+
// ! Cable_Vertex : Constructeur                                                       !
// +-----------------------------------------------------------------------------------+
WNG::Cable_Vertex::Cable_Vertex(const int floor_id1, const int floor_id2, const Point _Vertex)
{
	this->_PairFloor = std::make_pair(floor_id1, floor_id2);
	this->_Vertex = _Vertex;
}

// +-----------------------------------------------------------------------------------+
// ! Cable_Vertex : operator=                                                          !
// +-----------------------------------------------------------------------------------+
WNG::Cable_Vertex& WNG::Cable_Vertex::operator= (const Cable_Vertex &rh)
{
	this->Init(rh._PairFloor.first, rh._PairFloor.second, rh._Vertex);
	return *this;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public Cable_Vertex::Save(std::ofstream& ofs)                                   !
// +-----------------------------------------------------------------------------------+
void WNG::Cable_Vertex::Save(std::ofstream& ofs) const
{
	_Vertex.Save(ofs);
	IO::binary_write(ofs, _PairFloor.first);
	IO::binary_write(ofs, _PairFloor.second);
}

// +-----------------------------------------------------------------------------------+
// ! Methode public Cable_Vertex::Load (std::ifstream& i)                                    !
// +-----------------------------------------------------------------------------------+
void WNG::Cable_Vertex::Load(std::ifstream& ifs)
{
	_Vertex.Load(ifs);
	IO::binary_read(ifs, _PairFloor.first);
	IO::binary_read(ifs, _PairFloor.second);
}

//********************************* WNG_CablePort **************************************************************

// +-----------------------------------------------------------------------------------+
// ! WNG_CablePort : Constructeur                                                       !
// +-----------------------------------------------------------------------------------+
WNG::WNG_CablePort::WNG_CablePort(const int floor_id, const int device_id, const WNG_DasPort device_port)
{
	Init(floor_id, device_id, device_port);
}

// +-----------------------------------------------------------------------------------+
// ! WNG_CablePort : operator=                                                          !
// +-----------------------------------------------------------------------------------+
WNG::WNG_CablePort& WNG::WNG_CablePort::operator= (const WNG_CablePort &rh)
{
	this->Init(rh._FloorId, rh._DeviceId, rh._DevicePort);
	_FloorCx = rh._FloorCx;
	return *this;
}



// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_CablePort::Save(std::ofstream& ofs)                                   !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_CablePort::Save(std::ofstream& ofs) const
{
	IO::binary_write(ofs, _FloorId);
	IO::binary_write(ofs, _DeviceId);
	_DevicePort.SaveProject(ofs);
	_FloorCx.Save(ofs);

}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_CablePort::Load (std::ifstream& i)                                    !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_CablePort::Load(std::ifstream& ifs)
{
	IO::binary_read(ifs, _FloorId);
	IO::binary_read(ifs, _DeviceId);
	_DevicePort.LoadProject(ifs);
	_FloorCx.Load(ifs);

}


//********************************* WNG_Cable **************************************************************


// +-----------------------------------------------------------------------------------+
// ! WNG_Cable : operator=                                                          !
// +-----------------------------------------------------------------------------------+
WNG::WNG_Cable& WNG::WNG_Cable::operator= (const WNG_Cable& rh)
{
	this->_DasParam = rh._DasParam; this->_Id = rh._Id;
	this->_List_vertex = rh._List_vertex; this->_PairPort = rh._PairPort;
	return *this;
}

// +-----------------------------------------------------------------------------------+
// ! WNG_Cable : operator==                                                          !
// +-----------------------------------------------------------------------------------+
bool WNG::WNG_Cable::operator== (WNG_Cable const& rh)
{
	bool result = (this->_DasParam == rh._DasParam && this->_Id == rh._Id &&
		this->_PairPort.first == rh._PairPort.first &&
		this->_PairPort.second == rh._PairPort.second);
	if (result)
	{
		for (unsigned int i = 0; i < _List_vertex.size(); i++)
		{
			if (!(this->_List_vertex[i] == rh._List_vertex[i]))
			{
				result = false;
				break;
			}
		}
	}
	return result;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::Save(std::ofstream& ofs)                                   !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::Save(std::ofstream& ofs) const
{
	IO::binary_write(ofs, _Id);
	_DasParam.SaveProject(ofs);
	_PairPort.first.Save(ofs);
	_PairPort.second.Save(ofs);
	IO::binary_write(ofs, this->_List_vertex.size());
	for (const auto& vert : _List_vertex)
		vert.Save(ofs);

}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::Load (std::ifstream& i)                                    !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::Load(std::ifstream& ifs)
{
	IO::binary_read(ifs, _Id);
	_DasParam.LoadProject(ifs);
	_PairPort.first.Load(ifs);
	_PairPort.second.Load(ifs);
	int listversize;
	IO::binary_read(ifs, listversize);
	for (int i = 0; i < listversize; ++i)
	{
		Cable_Vertex ver;
		ver.Load(ifs);
		_List_vertex.push_back(ver);
	}
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::Load (std::ifstream& i)                                    !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::Init(const int id, const WNG_DasParam& dasParam, const std::vector<Cable_Vertex>& list_vertex, 
	const std::pair<WNG_CablePort, WNG_CablePort>& pairePort) 
{
	_Id = id; _DasParam = dasParam; _PairPort = pairePort; _List_vertex = list_vertex;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::Load (std::ifstream& i)                                 !
// !  - Initialisation d'une extremit�s du cable (b_first = true -> premiere extremit�)!
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::SetCablePort(const bool b_first, const int floor_id, const int device_id, const WNG_DasPort& device_port)
{
	if (b_first)
		_PairPort.first.Init(floor_id, device_id, device_port);
	else
		_PairPort.second.Init(floor_id, device_id, device_port);
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::Move(const double dx, const double dy)                                !
// !  - Move de tous les vertex suivant dx, dy                                         !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::Move(const double dx, const double dy)
{
	for (auto& it_vertex : _List_vertex)
	{
		it_vertex._Vertex.Move(dx, -dx);
	}

}

// +----------------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::InsertVertex(const int position, const Cable_Vertex& vertex  !
// !  - Insert un vertex apr�s le vertex d'indice position                                  !
// +----------------------------------------------------------------------------------------+
void WNG::WNG_Cable::InsertVertex(const int position, const Cable_Vertex& vertex)
{
	if (position >= (int)_List_vertex.size())
		_List_vertex.push_back(vertex);
	else
	{
		std::vector<Cable_Vertex>::iterator it = _List_vertex.begin();
		for (int i = 0; i < position; ++i)
			++it;
		//++it;
		_List_vertex.insert(it, vertex);
	}
}


// +----------------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::ModifyVertex(const int position, const Cable_Vertex& vertex) !
// !  - Modifie le vertex d'indice position                                                 !
// +----------------------------------------------------------------------------------------+
void WNG::WNG_Cable::ModifyVertex(const int position, const Cable_Vertex& vertex)
{
	if (position < (int)_List_vertex.size())
	{
		_List_vertex[position].operator=(vertex);
	}
}

// +----------------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::ModifyVertex(const int position, const Cable_Vertex& vertex) !
// !  - Delete le vertex d'indice position                                                 !
// +----------------------------------------------------------------------------------------+
void WNG::WNG_Cable::DeleteVertex(const int position)
{
	if (position < (int)_List_vertex.size())
	{
		_List_vertex.erase(_List_vertex.begin() + position);
	}
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetVertexIndice(const double x, const double y) const   !
// !  - Retourne indice du vertex proche de x,y, -1 si >0.05                            !
// +-----------------------------------------------------------------------------------+
int WNG::WNG_Cable::GetVertexIndice(const double x, const double y) const
{
	int result = -1, indice = 0;;
	double dist = 9999, dist_temp =9999;
	for (const auto& it_vertex : _List_vertex)
	{
#ifdef WIN32
		if (abs(it_vertex.GetX() - x) < 0.05 && abs(it_vertex.GetY() - y)< 0.05)
		{
			dist_temp = sqrt((it_vertex.GetX() - x) * (it_vertex.GetX() - x) + (it_vertex.GetY() - y) * (it_vertex.GetY() - y));
			if (dist_temp < dist)
			{
				dist = dist_temp;
				result = indice;
			}
		}
#elif defined __ANDROID__
		if (fabs(it_vertex.GetX() - x) < 0.05 && fabs(it_vertex.GetY() - y)< 0.05)
		{
			dist_temp = sqrt((it_vertex.GetX() - x) * (it_vertex.GetX() - x) + (it_vertex.GetY() - y) * (it_vertex.GetY() - y));
			if (dist_temp < dist)
			{
				dist = dist_temp;
				result = indice;
			}
		}
#endif

		indice++;
	}
	return result;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::SetFloor(const int floor_id)                            !
// !  - Setting _PairPort et _PairFloor de tous les vertex avec valeur de floor_id     !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::SetFloor(const int floor_id)
{
	for (auto& it_vertex : _List_vertex)
	{
		it_vertex._PairFloor.first = floor_id;
		it_vertex._PairFloor.second = floor_id;
	}
	_PairPort.first._FloorId = floor_id;
	_PairPort.second._FloorId = floor_id;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetListVertexFloor(const int floor_id) const            !
// !  - Retourne la liste des vertex du cable pr�sent sur l'�tage floor_id             !
// +-----------------------------------------------------------------------------------+
std::vector<WNG::Cable_Vertex> WNG::WNG_Cable::GetListVertexFloor(const int floor_id) const
{
	std::vector<Cable_Vertex> list_vertex;
	for (const auto& it_vertex : _List_vertex)
	{
		if (it_vertex._PairFloor.first == floor_id || it_vertex._PairFloor.second == floor_id)
		{
			list_vertex.push_back(it_vertex);
		}
	}
	return list_vertex;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetDeviceId(const bool b_first) const                   !
// !  - Retourne l'id du device de l'extr�mit� du cable                                !
// !    (b_first = true -> premiere extremit�)                                         !
// +-----------------------------------------------------------------------------------+
int WNG::WNG_Cable::GetDeviceId(const bool b_first) const
{
	return b_first ? _PairPort.first._DeviceId : _PairPort.second._DeviceId;
}
// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetCablePort(const bool b_first) const                  !
// !  - Retourne le WNG_CablePort de l'extr�mit� du cable                              !
// !    (b_first = true -> premiere extremit�)                                         !
// +-----------------------------------------------------------------------------------+
WNG::WNG_CablePort WNG::WNG_Cable::GetCablePort(const bool b_first) const
{
	return b_first ? _PairPort.first : _PairPort.second;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetDevicePort(const bool b_first) const                   !
// !  - Retourne le WNG_DasPort du device de l'extr�mit� du cable                      !
// !    (b_first = true -> premiere extremit�)                                         !
// +-----------------------------------------------------------------------------------+
WNG::WNG_DasPort WNG::WNG_Cable::GetDevicePort(const bool b_first) const
{
	return b_first ? _PairPort.first._DevicePort : _PairPort.second._DevicePort;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::SetDevicePort(const bool b_first, const WNG::WNG_DasPort& dasPort) !
// !  - Setting le WNG_DasPort du device de l'extr�mit� du cable                      !
// !    (b_first = true -> premiere extremit�)                                         !
// +-----------------------------------------------------------------------------------+
void WNG::WNG_Cable::SetDevicePort(const bool b_first, const WNG::WNG_DasPort& dasPort)
{
	if (b_first)
		_PairPort.first._DevicePort = dasPort;
	else
		_PairPort.second._DevicePort = dasPort;
}

// +-----------------------------------------------------------------------------------+
// ! Methode public WNG_Cable::GetDevicePortTypeName(const bool b_first) const         !
// !  - Retourne lle TypeName du device de l'extr�mit� du cable                        !
// !    (b_first = true -> premiere extremit�)                                         !
// +-----------------------------------------------------------------------------------+
 std::string WNG::WNG_Cable::GetDevicePortTypeName(const bool b_first) const
{
	 return b_first ? _PairPort.first._DevicePort._PortTypeName : _PairPort.second._DevicePort._PortTypeName;
}


