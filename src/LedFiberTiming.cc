#include "LedFiberTiming.hh"

double cLight      = 2.99792458e-1;   // speed of light in vacuum [mm/ps]



Fiber FiberInit(const double& length, const double& radius, const double& att,
                const bool& isVertical)
{
  Fiber f;
  f.lengthTotal = length;
  f.radius = radius;
  f.attenuation = att;
  f.refrIndCore = 1.842;
  f.refrIndClad = 1.000;
  f.refrIndAir  = 1.000;
  f.refrIndDet  = 4.000;
  f.position.SetX(0.);
  f.position.SetY(0.);
  f.position.SetZ(0.);
  if( isVertical )
  {
    f.direction.SetX(0);
    f.direction.SetY(1);
    f.direction.SetZ(0);
  }
  else
  {
    f.direction.SetX(1);
    f.direction.SetY(0);
    f.direction.SetZ(0);
  }
  return f;
}



double fresnelReflection(const double& th, const double& R)
{
  // reflection probablity for unpolarized photons
  //   th - incident angle wrt to normal
  //   R = n1/n2 where 
  //       n1 is refractive index of fiber
  //       n2 is refractive index of outside media
  //  
  double u = 1. - R*R*sin(th)*sin(th);
  if(u<=0.) u=0.;
  u = sqrt(u);
  double Rs = (R*cos(th) - u)/(R*cos(th) + u);
  Rs = Rs * Rs;
  double Rp = (R*u - cos(th))/(R*u + cos(th));
  Rp = Rp * Rp;
  return 0.5*(Rs+Rp);
}



Travel GetTimeAndProbability(Photon& ph, const Fiber* fib, const double& prodTime)
{
  Travel result;
  for(int i = 0; i < 10; ++i)
  {
    result.prob[i] =  0.;
    result.time[i] = -1.;
  }
  result.nmax = 5;
  
  double cosTheta = fib->direction * ph.direction;
  double theta = acos(cosTheta);
  double absTheta = acos(fabs(cosTheta));

  double probReflectFace  = fresnelReflection(absTheta, fib->refrIndCore/fib->refrIndAir);
  double probReflectRear  = fresnelReflection(absTheta, fib->refrIndCore/fib->refrIndAir);

  // Total internal reflection at the face and the rear, photon will not reach a photo-detector
  if( (1.-probReflectFace) < 1.e-9 &&
      (1.-probReflectRear) < 1.e-9 )
    return result;

  // fiber axis
  TVector3 b = fib->position + fib->direction * fib->direction.Dot(ph.position - fib->position);
  // direction of a photon in transverse plane
  TVector3 v = ph.direction - fib->direction * fib->direction.Dot(ph.direction);
  // distance from the photon trajectory and the fiber axis in transverse plane
  TVector3 d = (b - ph.position) - v * (v.Dot(b - ph.position) / v.Mag2());
  // skew angle in transverse plane
  double cosGamma = sqrt( 1. - d.Mag2()/(fib->radius*fib->radius) );  
  // check if there is a total internal reflection off the fiber walls
  if( acos(cosGamma*sin(theta))<asin(fib->refrIndClad/fib->refrIndCore) ) return result;

  double relDistance = 0.;
  double probReflections = 1.;
  if( cosTheta > 0 ) relDistance = 1. - ph.dist;
  else               relDistance = ph.dist;
  
  for(int i = 0; i < result.nmax; ++i)
  {    
    double length = relDistance * fib->lengthTotal / fabs(cosTheta);
    result.time[i] = prodTime + length / cLight * fib->refrIndCore;
    
    if( cosTheta > 0 ) result.prob[i] =  exp( -length / fib->attenuation ) * probReflections * (1.-probReflectRear);
    else               result.prob[i] =  exp( -length / fib->attenuation ) * probReflections * (1.-probReflectFace);
    
    
    //if( fabs(ph.position.y()) > 100 )
    //std::cout << "result.prob[i]: " << result.prob[i] << "   y: " << ph.position.y() << "   py: " << ph.direction.y() << "   dist: " << ph.dist << "   relDistance: " << relDistance << "   length: " << length/10. << "  exp( -length / fib->attenuation ): " <<  exp( -length / fib->attenuation ) << "   probReflections: " << probReflections << "   (1.-probReflectRear): " << (1.-probReflectRear) << std::endl;  
    
    relDistance += 1.;
    if( cosTheta > 0 ) probReflections *= probReflectRear;
    else               probReflections *= probReflectFace;
    cosTheta *= -1.;
  }
  return result;    
}
