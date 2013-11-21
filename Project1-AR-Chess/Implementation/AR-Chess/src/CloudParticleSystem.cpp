#include "CloudParticleSystem.h"


EmitterUpdateCallback::EmitterUpdateCallback(PointPlacer* pointPlacer, MatrixTransform* boardTrackerMT, MatrixTransform* selectorTrackerMT) :
	_pointPlacer(pointPlacer),
	_boardTrackerMT(boardTrackerMT),
	_selectorTrackerMT(selectorTrackerMT) {}


void EmitterUpdateCallback::operator() (Node* node, NodeVisitor* nodeVisitor) {
	Vec3 offset = _selectorTrackerMT->getMatrix().getTrans() - _boardTrackerMT->getMatrix().getTrans();
	offset.z() += TRACKER_SELECTOR_PARTICLE_HEIGHT_OFFSET;
	_pointPlacer->setCenter(offset);

	traverse(node, nodeVisitor);
}


CloudParticleSystem::CloudParticleSystem(Group* scene, MatrixTransform* boardTrackerMT, MatrixTransform* selectorTrackerMT,
	string particleTextureFile,
	bool useEmissiveParticles,
	bool useLightingInParticles,
	rangef particleSizeRange,
	double particleLifetimeInSeconds,
	float particleMass,
	rangev4 particleColorRange,
	Vec2 particleCreationRateRange,
	Vec2 particleRadialShooterThetaRange,
	Vec2 particleRadialInitialSpeedRange,
	Vec3 particleFluidFrictionWind,
	float particleGravityAcceleration) {

	// particle system
	ParticleSystem* dustParticleSystem = new ParticleSystem();
	Geode *geode = new Geode();
	geode->addDrawable(dustParticleSystem);
	this->addChild(geode);

	dustParticleSystem->setDefaultAttributes(particleTextureFile, useEmissiveParticles, useLightingInParticles);

	// default particle
	Particle smokeParticle;
	smokeParticle.setSizeRange(particleSizeRange); // meters
	smokeParticle.setLifeTime(particleLifetimeInSeconds); // seconds
	smokeParticle.setMass(particleMass);
	smokeParticle.setColorRange(particleColorRange);
	dustParticleSystem->setDefaultParticleTemplate(smokeParticle);

	// particles updater
	ParticleSystemUpdater *dustSystemUpdater = new ParticleSystemUpdater();
	dustSystemUpdater->addParticleSystem(dustParticleSystem);
	this->addChild(dustSystemUpdater);

	// particles emitter
	ModularEmitter *emitter = new ModularEmitter();
	emitter->setParticleSystem(dustParticleSystem);
	this->addChild(emitter);

	// particles counter
	RandomRateCounter *dustRate = static_cast<RandomRateCounter *>(emitter->getCounter());
	dustRate->setRateRange(particleCreationRateRange.x(), particleCreationRateRange.y());

	// particles placer
	PointPlacer* placer = static_cast<PointPlacer *>(emitter->getPlacer());
	this->setUpdateCallback(new EmitterUpdateCallback(placer, boardTrackerMT, selectorTrackerMT));

	// particles shooter
	RadialShooter* shooter = static_cast<RadialShooter *>(emitter->getShooter());
	shooter->setThetaRange(particleRadialShooterThetaRange.x(), particleRadialShooterThetaRange.y()); // radians, relative to Z axis.
	shooter->setInitialSpeedRange(particleRadialInitialSpeedRange.x(), particleRadialInitialSpeedRange.y()); // meters/second
	emitter->setShooter(shooter);

	ModularProgram *program = new ModularProgram();
	program->setParticleSystem(dustParticleSystem);
	scene->addChild(program);

	// particles friction
	FluidFrictionOperator *airFriction = new FluidFrictionOperator();
	airFriction->setFluidToAir();
	airFriction->setWind(particleFluidFrictionWind);
	program->addOperator(airFriction);

	// particles acceleration
	AccelOperator *accelUp = new AccelOperator();
	accelUp->setToGravity(particleGravityAcceleration);
	program->addOperator(accelUp);
}


CloudParticleSystem::~CloudParticleSystem() {}



