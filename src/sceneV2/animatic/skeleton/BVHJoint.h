#ifndef __BVHJOINT_H__
#define __BVHJOINT_H__

#include <string>
#include <vector>
#include <cassert>

#include <iostream>
#include <fstream>

//#include "BVH.h"
#include "BVHChannel.h"
#include "BVHAxis.h"

/** @addtogroup BVH
	@{
*/

namespace TTe {

	class BVH;

/** @brief Motion capture bone and bone animation

	@remark
		Contains the description and the evolution of a bone.
*/
class BVHJoint
{
	friend class TTe::BVH;
public:
	//! @brief Constructor
	BVHJoint(const std::string& name, int parent, BVH& bvh, int id);
	//! Destructor (recursive)
	~BVHJoint() {}

	// copy constructor and operator
	BVHJoint(const BVHJoint& joint) : m_bvh(joint.m_bvh){
		m_name = joint.m_name;
		m_parentId = joint.m_parentId;
		m_id = joint.m_id;
		m_offset[0] = joint.m_offset[0];
		m_offset[1] = joint.m_offset[1];
		m_offset[2] = joint.m_offset[2];
		m_channels = joint.m_channels;
		m_childs = joint.m_childs;
	}

	BVHJoint & operator=(const BVHJoint & joint);

	//! Return the name of the Joint
	std::string getName(void) const { return m_name;  }
	//! Modify the name of the Joint
	void setName(const std::string& name) { m_name = name;  }

	//! return id
	int getId() const { return m_id; }

	//! Return the offset of the joint
	void getOffset(float& x, float& y, float& z) const { x = m_offset[0]; y = m_offset[1]; z = m_offset[2]; }
	//! Modify the offset of the joint
	void setOffset(float x, float y, float z) { m_offset[0] = x; m_offset[1] = y; m_offset[2] = z; }

	//! Return the number of channels in the Joint
	int getNumberOfChannel(void) const { return (int)m_channels.size();  }
	//! Return the i-th channel of the Joint
	const TTe::BVHChannel& getChannel(int i) const  { assert(i >= 0); assert(i < (int)m_channels.size()); return m_channels[i]; }

	//! Return the i-th channel of the Joint
	TTe::BVHChannel& getChannel(int i) { assert(i >= 0); assert(i < (int)m_channels.size()); return m_channels[i]; }
	//! modify the i-th channel of the Joint
	void setChannel(int i, const TTe::BVHChannel& channel);
	//! Add channel
	void addChannel(const TTe::BVHChannel& channel)	{ m_channels.push_back(channel);  }
	//! Remove channel
	void removeChannel(const TTe::BVHChannel& channel);
	//! Remove channel
	void removeChannel(int i);

	//! Return the number of child
	int getNumberOfChildren(void) const		{ return (int)m_childs.size();  }
	//! Return a child
	const TTe::BVHJoint& getChild(int i) const;
	//! Return the i-th child Id in the vector<Joint> store in the BVH
	int getChildId(int i) const 	{ assert(i >= 0); assert(i < (int)m_childs.size()); return m_childs[i]; }
	//! Add child
	void addChild(int i);

	//! return true if the Joint is the root
	bool isRoot() const { return getParentId()<0;  }

	//! Return the parent of the Joint
	const BVHJoint& getParent(void) const;
	//! Return the parent indice of the Joint
	int getParentId(void) const	{ return m_parentId;  }
	//! Modify the parent of the joint
	void setParentId(int parent);

	//! Return the bvh
	const TTe::BVH& getBVH(void) const;

	//! Scaling the joint
	void scale(float factor);
	//! Rotate the bvh (recursive)
	void rotate90(AXIS axis, bool cw);

	//! dump (recursive)
	friend std::ostream& operator << (std::ostream& os, const BVHJoint& joint);

	//! init channels from a opened file
	void initChannel(std::ifstream& stream);

protected:

	//! Joint name
	std::string m_name;

	//! The parent
	int m_parentId;

	//! The BVH
	TTe::BVH& m_bvh;

	//! Id in the vector<joint> of BVH
	int m_id;

	//! Offset vector
	float m_offset[3];

	//! Channels
	std::vector<TTe::BVHChannel> m_channels;

	//! Childs
	std::vector<int> m_childs;
};

} // namespace

/** @}
*/

#endif //__JOINT_H__
