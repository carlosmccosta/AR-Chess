#pragma once


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// OSG include
#include <osg/MatrixTransform>

#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgParticle/PointPlacer>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/ModularProgram>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/AccelOperator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

// std includes
#include <string>

// project includes
#include "Configs.h"


// namespace specific imports to avoid namespace pollution
using osg::MatrixTransform;
using osg::Vec2;
using osg::Vec3;
using osg::Vec4;
using osg::Group;
using osg::Node;
using osg::Geode;
using osg::NodeVisitor;
using osg::NodeCallback;
using osgParticle::PointPlacer;
using osgParticle::Particle;
using osgParticle::ParticleSystem;
using osgParticle::ParticleSystemUpdater;
using osgParticle::ModularEmitter;
using osgParticle::RandomRateCounter;
using osgParticle::PointPlacer;
using osgParticle::RadialShooter;
using osgParticle::ModularProgram;
using osgParticle::FluidFrictionOperator;
using osgParticle::AccelOperator;
using osgParticle::rangef;
using osgParticle::rangev4;
using std::string;
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <includes> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



class EmitterUpdateCallback : public NodeCallback {
	public:
		EmitterUpdateCallback(PointPlacer* pointPlacer, MatrixTransform* boardTrackerMT, MatrixTransform* selectorTrackerMT);


		void operator() (Node* node, NodeVisitor* nodeVisitor);

	private:
		PointPlacer* _pointPlacer;
		MatrixTransform* _boardTrackerMT;
		MatrixTransform* _selectorTrackerMT;
};



class CloudParticleSystem : public Group {
	public:
		CloudParticleSystem(Group* scene, MatrixTransform* boardTrackerMT, MatrixTransform* selectorTrackerMT,
			string particleTextureFile = TRACKER_SELECTOR_PARTICLE_IMG,
			bool useEmissiveParticles = true,
			bool useLightingInParticles = true,
			rangef particleSizeRange = rangef(2.0f, 14.0f),
			double particleLifetimeInSeconds = 1.5,
			float particleMass = 0.02f,
			rangev4 particleColorRange = rangev4(Vec4(0.5f, 0.5f, 0.5f, 0.8f), Vec4(0, 0, 0, 0.2f)),
			Vec2 particleCreationRateRange = Vec2(75, 150),
			Vec2 particleRadialShooterThetaRange = Vec2(0, osg::DegreesToRadians(30.0)),
			Vec2 particleRadialInitialSpeedRange = Vec2(0, 20),
			Vec3 particleFluidFrictionWind = Vec3(0, 0, 50),
			float particleGravityAcceleration = 0.8);
		virtual ~CloudParticleSystem();

	private:
		PointPlacer* placer;
};