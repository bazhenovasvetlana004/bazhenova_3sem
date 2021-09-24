#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

struct vector3 {
    double x;
    double y;
    double z;
};

class Particle {
protected:
    double mass = 0;
    double charge = 0;
    vector3 coordinate = {0, 0, 0};
    vector3 velocity = {0, 0,0};
    vector3 force = {0, 0, 0};
    std::string name;

public:
    Particle() = default;
    virtual void interaction(Particle& p)
    {
    };
    void setForceDefault()
    {
        force = { 0,0,0 };
    };
    void Step(double time)
    {
        coordinate.x = coordinate.x + velocity.x * time + force.x / mass * time * time / 2;
        coordinate.y = coordinate.y + velocity.y * time + force.y / mass * time * time / 2;
        coordinate.z = coordinate.z + velocity.z * time + force.z / mass * time * time / 2;
        velocity.x = velocity.x + force.x / mass * time;
        velocity.y = velocity.y + force.y / mass * time;
        velocity.z = velocity.z + force.z / mass * time;
    };
    std::string getName() const
    {
        return name;
    };
    void setName(std::string newname)
    {
        name = newname;
    };
    double getEnergy() const
    {
        double Velocity = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
        return mass * Velocity * Velocity / 2;
    };
    vector3 getCoords()
    {
        return coordinate;
    };
    double getCharge()
    {
        return charge;
    };
    vector3 getForce()
    {
        return force;
    }
};

class Electron : public Particle {
public:
    explicit Electron(vector3 veloc, vector3 coords)
    {
        name = "electron";
        charge = -1;
        mass = 1;
        velocity = veloc;
        coordinate = coords;
    };
    void interaction(Particle& p) override
    {
        vector3 secondParticleCoords = p.getCoords();
        double Distance = pow(secondParticleCoords.x - coordinate.x, 2) + pow(secondParticleCoords.y - coordinate.y, 2) + pow(secondParticleCoords.z - coordinate.z, 2);
        double ForceValue = charge * p.getCharge() / Distance;
        double fx = -(secondParticleCoords.x - coordinate.x) * ForceValue / sqrt(Distance);
        double fy = -(secondParticleCoords.y - coordinate.y) * ForceValue / sqrt(Distance);
        double fz = -(secondParticleCoords.z - coordinate.z) * ForceValue / sqrt(Distance);
        force.x = force.x + fx;
        force.y = force.y + fy;
        force.z = force.z + fz;
    };
    ~Electron()
    {};
};

class Proton : public Particle {
public:
    explicit Proton(vector3 veloc, vector3 coords)
    {
        name = "proton";
        charge = 1;
        mass = 10;
        velocity = veloc;
        coordinate = coords;
    };
    void interaction(Particle& p) override
    {
        vector3 secondParticleCoords = p.getCoords();
        double Distance = pow(secondParticleCoords.x - coordinate.x, 2) + pow(secondParticleCoords.y - coordinate.y, 2) + pow(secondParticleCoords.z - coordinate.z, 2);
        double ForceValue = charge * p.getCharge() / Distance;
        double fx = -(secondParticleCoords.x - coordinate.x) * ForceValue / sqrt(Distance);
        double fy = -(secondParticleCoords.y - coordinate.y) * ForceValue / sqrt(Distance);
        double fz = -(secondParticleCoords.z - coordinate.z) * ForceValue / sqrt(Distance);
        force.x = force.x + fx;
        force.y = force.y + fy;
        force.z = force.z + fz;
    };
    ~Proton()
    {};
};

class World {
private:
    std::vector<Particle*> v;
    int electrons = 0;
    int protons = 0;
public:
    void addParticle(Particle& p)
    {
        std::string prevname = p.getName();
        if (prevname == "electron")
        {
            electrons++;
            prevname = prevname + std::to_string(electrons);
        }
        if (prevname == "proton")
        {
            protons++;
            prevname = prevname + std::to_string(protons);
        }
        p.setName(prevname);
        v.push_back(&p);
    }

    void deleteParticle(Particle& p)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if(v[i]->getName() == p.getName())
                v.erase(v.begin() + i);
        }
    };

    void Step(double time)
    {
        for (int i = 0; i < v.size(); i++)
        {
            v[i]->setForceDefault();
        }
        for (int i = 0; i < v.size(); i++)
        {
            for (int j = 0; j < v.size(); j++)
            {
                if (i != j)
                {
                    v[i]->interaction(*v[j]);
                }
            }
        }
        for (int i = 0; i < v.size(); i++)
        {
            v[i]->Step(time);
        }
    };

    std::vector<Particle*>* getArray()
    {
        return &v;
    };
};

std::ostream& operator<<(std::ostream& os, World& w)
{
    for (int i = 0; i < w.getArray()->size(); i++)
    {
        os << "Particle " << i + 1 << ": " << w.getArray()->at(i)->getName() << std::endl <<
            "x = " << w.getArray()->at(i)->getCoords().x << ", y = " << w.getArray()->at(i)->getCoords().y <<
            ", z = " << w.getArray()->at(i)->getCoords().z << std::endl <<
            "fx = " << w.getArray()->at(i)->getForce().x << ", fy = " << w.getArray()->at(i)->getForce().y <<
            ", fz = " << w.getArray()->at(i)->getForce().z << std::endl;

    }
    return os;
};

int main() {
    setlocale(LC_ALL, "Russian");
    vector3 acoords = { 0,10,0 };
    vector3 aveloc = { 0,0,0 };
    Electron a(aveloc,acoords);
    vector3 bcoords = { 5,0,0 };
    vector3 bveloc = { 0,0,0 };
    Electron b(bveloc, bcoords);
    vector3 ccoords = { -5,0,0 };
    vector3 cveloc = { 0,0,0 };
    Electron c(cveloc, ccoords);
    vector3 ocoords = { 0,5,0 };
    vector3 oveloc = { 0,0,0 };
    Proton o(oveloc, ocoords);
    World triangle;
    triangle.addParticle(a);
    triangle.addParticle(b);
    triangle.addParticle(c);
    triangle.addParticle(o);
    for (int i = 0; i < 100000; i++)
    {
        triangle.Step(0.0001);
        
        if (i % 1000 == 0)
        {
            Sleep(1000);
            std::cout << triangle;
        }
    }
    
}