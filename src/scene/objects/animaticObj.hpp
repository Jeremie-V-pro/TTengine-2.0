#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include "scene/object.hpp"
#include "scene/objects/animatic/BVH.h"
namespace TTe {
class AnimaticObj {
   public:
    struct SkeletonJoint {
        int m_parentId;   // Le num�ro du p�re dans le tableau de CAJoint de CASkeleton
        glm::mat4 m_l2w;  // La matrice passant du rep�re de l'articulation vers le monde
        glm::mat4 normal_m_l2w;
    };

    //! Cr�er un squelette ayant la m�me structure que d�finit dans le BVH c'est � dire
    //! creer le tableau de SkeletonJoint � la bonne taille, avec les parentId initials� pour chaque case
    void init(const BVH& bvh);

    //! Renvoie la position de l'articulation i en multipliant le m_l2w par le Point(0,0,0)
    glm::vec3 getJointPosition(int i) const;

    //! Renvoie l'identifiant de l'articulation p�re de l'articulation num�ro i
    int getParentId(const int i) const;

    //! Renvoie le nombre d'articulation
    int numberOfJoint() const { return (int)m_joints.size(); }

    //! Positionne ce squelette dans la position n du BVH.
    //! Assez proche de la fonction r�cursive (question 1), mais range la matrice (Transform)
    //! dans la case du tableau. Pour obtenir la matrice allant de l'articulation local vers le monde,
    //! il faut multiplier la matrice allant de l'articulation vers son p�re � la matrice du p�re allant de
    //! l'articulation du p�re vers le monde.
    void setPose(const BVH& bvh, int frameNumber);

    //! Positionne ce squelette entre la position frameNbSrc du BVH Src et la position frameNbDst du bvh Dst
    // void setPoseInterpolation(const BVH& bvhSrc, int frameNbSrc, const BVH& bvhDst, int frameNbDst, float t);

    //! Positionne ce squelette entre la position frameNbSrc du BVH Src et la position frameNbDst du bvh Dst
    //! idem � setPoseInterpolation mais interpole avec des quaternions sur chaque articulations
    // void setPoseInterpolationQ(const BVH& bvhSrc, int frameNbSrc, const BVH& bvhDst, int frameNbDst, float t);

    //! Calcule la distance entre deux poses
    //! precond: les deux squelettes doivent avoir le
    //! m�me nombre d'articulations (m�me structure d'arbre)
    //! ==> Sera utile lors de la construction du graphe d'animation
    // friend float distance(const CASkeleton& a, const CASkeleton& b);
   private:
    std::vector<Object> m_joints;
};
}  // namespace TTe