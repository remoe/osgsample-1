#include <osg/Notify>
#include <osg/ref_ptr>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Program>
#include <osg/Shader>
#include <osgViewer/Viewer> 
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

static const char* vert_source = {
	"$OSG_GLSL_VERSION\n"
	"$OSG_PRECISION_FLOAT\n"
	"\n"
	"$OSG_VARYING_OUT vec2 texCoord;\n"
	"\n"
	"void main(void)\n"
	"{\n"
	"    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
	"    texCoord = gl_MultiTexCoord0.xy;\n"
	"}\n"
	"\n"
};

static const char* frag_source = {
	"$OSG_GLSL_VERSION\n"
	"uniform sampler2D tex;\n"
	"in vec2 texCoord;\n"
	"void main(void)\n"
	"{\n"
	"    gl_FragColor = texture(tex, texCoord).rgba;\n"
	"}\n"
};

osg::Image* make_checker_image(unsigned int width, unsigned height)
{
	osg::Image* img = new osg::Image();
	img->allocateImage(width,height,1,GL_RGB, GL_UNSIGNED_BYTE);

	int i, j, c;

	srand(time(NULL));

	osg::Vec3f color(rand() % 128, rand() % 128, rand() % 128);

	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0));
			unsigned char* pixel = img->data(i,j);
			pixel[0] = c * color.x();
			pixel[1] = c * color.y();
			pixel[2] = c * color.z();
		}
	}

	return img;
}

void create_tex_coords(osg::Vec2Array* uv, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		float u = ((float)rand() / (RAND_MAX));
		float v = ((float)rand() / (RAND_MAX));
		uv->push_back(osg::Vec2(u, v));
	}
}

class TexturedCube : public osg::Geometry
{
public:
	TexturedCube()
	{
		osg::Vec4Array* cAry = new osg::Vec4Array;
		setColorArray(cAry, osg::Array::BIND_OVERALL);
		cAry->push_back(osg::Vec4(1, 1, 1, 1));

		osg::Vec3Array* vert = new osg::Vec3Array();
		setVertexArray(vert);

		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));


		for (int i = 0;i < 9;++i) {
			osg::Vec2Array* uv = new osg::Vec2Array();
			setTexCoordArray(i, uv);
			create_tex_coords(uv, vert->size());
		}
    
		addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vert->size()));

		osg::StateSet* ss = getOrCreateStateSet();

		// set up the texture state.
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		texture->setImage(make_checker_image(128,128));
		ss->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

		osg::Program* program = new osg::Program;
		program->setName("sample");
		program->addShader(new osg::Shader(osg::Shader::VERTEX, vert_source));
		program->addShader(new osg::Shader(osg::Shader::FRAGMENT, frag_source));
		ss->setAttributeAndModes(program, osg::StateAttribute::ON);
	}

	osg::VertexArrayState* createVertexArrayStateImplementation(osg::RenderInfo& renderInfo) const
	{
		osg::VertexArrayState* vas = osg::Geometry::createVertexArrayStateImplementation(renderInfo);

		// make sure we have array dispatchers for the multipass coords
		vas->assignTexCoordArrayDispatcher(_texCoordList.size());

		return vas;
	}
};

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osg::Geode* root(new osg::Geode);
	root->addDrawable(new TexturedCube);

	osgViewer::Viewer viewer(arguments);
	viewer.setSceneData(root);
	return viewer.run();
}