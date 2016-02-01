// Irrlicht Engine Obstacle Models Test

#include <iostream>
#include <irrlicht.h>



using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



int main()
{
    SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
    params.AntiAlias = 8;
    params.DriverType = video::EDT_OPENGL;
    params.WindowSize = core::dimension2d<u32>(1280, 960);
    IrrlichtDevice *device = createDeviceEx(params);

    //IrrlichtDevice *device =
    //   createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 960), 16,
    //                 false, false, false, 0);
    if (!device)
        return 1;

    device->setWindowCaption(L"Hello, World! - Irrlicht Engine Test");

    IVideoDriver*driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello, world! This is the Irrlicht Software Renderer", rect<s32>(10, 10, 260, 22), true);

    IAnimatedMesh* mesh = smgr->getMesh("objects/objects.3ds");
    if (!mesh)
    {
        device->drop();
        return 1;
    }
    //IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
    IMeshSceneNode * node = 0;
        // The Quake mesh is pickable, but doesn't get highlighted.
    if (mesh)
        node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0);


    if (node)
    {
        node->setMaterialFlag(EMF_LIGHTING, false);
        //node->setMD2Animation(scene::EMAT_STAND);
        node->setScale(core::vector3df(20,20,20));
        //node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
    }


    /*
    So far so good, we've loaded the quake 3 level like in tutorial 2. Now,
    here comes something different: We create a triangle selector. A
    triangle selector is a class which can fetch the triangles from scene
    nodes for doing different things with them, for example collision
    detection. There are different triangle selectors, and all can be
    created with the ISceneManager. In this example, we create an
    OctreeTriangleSelector, which optimizes the triangle output a little
    bit by reducing it like an octree. This is very useful for huge meshes
    like quake 3 levels. After we created the triangle selector, we attach
    it to the node. This is not necessary, but in this way, we do not
    need to care for the selector, for example dropping it after we do not
    need it anymore.
    */

    scene::ITriangleSelector* selector = 0;

    if (node)
    {
        node->setPosition(core::vector3df(0,0,0));

        selector = smgr->createOctreeTriangleSelector(
                node->getMesh(), node, 128);
        node->setTriangleSelector(selector);
        // We're not done with this selector yet, so don't drop it.
    }

    ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	device->getCursorControl()->setVisible(false);
    scene::ISceneNodeAnimatorCollisionResponse* anim = smgr->createCollisionResponseAnimator(
    selector, camera, vector3df(5,5,5),
    vector3df(0,0,0), vector3df(0,0,0));

    if (selector)
    {
        selector->drop(); // As soon as we're done with the selector, drop it.
        camera->addAnimator(anim);
        //anim->drop();  // And likewise, drop the animator when we're done referring to it.
    }

    //smgr->setAmbientLight(video::SColorf(1.0,1.0,1.0,1));
    //ILightSceneNode* light1 = smgr->addLightSceneNode( 0, core::vector3df(20,20,20), video::SColorf(1.0f,1.0f,1.0f), 1.0f, 1 );


    //smgr->addCameraSceneNode(0,vector3df(0, 30, -40), vector3df(0,5,0));
	int lastFPS = -1;
    bool collision;
    while (device->run())
    {
        driver->beginScene(true, true, SColor(255, 100, 101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
        int fps = driver->getFPS();

			//if (lastFPS != fps)
			//{
				stringw str = L"Robosub Simulator[";
				str += driver->getName();

				str += "] FPS:";
				str += fps;
                collision = anim->collisionOccurred();
                if (collision)
                    str += "Collision!";
				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			//}
    }

    device->drop();
    return 0;
}
