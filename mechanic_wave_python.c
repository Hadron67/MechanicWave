//use python script to control mechanic wave
#define PYTHON_ERROR(msg) { PyErr_SetString(PyExc_RuntimeError, msg); return NULL; }
#define RETURN_NONE Py_INCREF(Py_None); return Py_None
#include "mechanic_wave_python.h"
#include "messagebox.h"
#include<Python.h>
#include<string.h>
#include<stdlib.h>
MechanicWave* SEAL(this_wave)=NULL;
char* mechanic_wave_scripterror="";
typedef struct _py_mechanicwave{
	PyObject_HEAD;
	MechanicWave* wave;
}Py_Mechanicwave;
typedef struct _py_Wavesource{
	PyObject_HEAD;
	Wavesource* source;
}Py_Wavesource;
static PyObject* py_wave_addsource(Py_Mechanicwave* self,PyObject* args);
static Py_Wavesource* py_wave_getsource(Py_Mechanicwave* self,PyObject* args);
//python methods for mechanic wave:
//returning Py_None is dangous,for some unknown reason 
//.....................................................................................
static int py_mechanicwave_init(Py_Mechanicwave* self,PyObject* args){
	printf("it works!\n");
	int height=0,width=0;
	float dx=0,v=0;
	if(!PyArg_ParseTuple(args,"iiff",&height,&width,&dx,&v)) return -1;
	self->wave=mechanic_wave_new(height,width,dx,v);
	return 0;
}
static void py_mechanicwave_free(Py_Mechanicwave* self){
	if(self->wave!=SEAL(this_wave)) mechanic_wave_free(self->wave);
	Py_TYPE(self)->tp_free((PyObject*)self);
}
static PyObject* py_mechanicwave_repr(Py_Mechanicwave* self){
	return Py_BuildValue("s","a mechanic wave.");
}
//.....................................................................................
static PyObject* py_wave_reset(Py_Mechanicwave* self,PyObject* args){
	mechanic_wave_reset(self->wave);
	RETURN_NONE;
}
//.....................................................................................
static PyObject* py_wave_removesources(Py_Mechanicwave* self,PyObject* args){
	if(self->wave->sources!=NULL) mechanic_wave_remove_source(self->wave);
	RETURN_NONE;
}
//.......................................................................................
static PyObject* py_wave_setv(Py_Mechanicwave* self,PyObject* args){
	int x,y;
	float value;
	if(!PyArg_ParseTuple(args,"iif",&x,&y,&value)) return NULL;
	if(x<0||y<0||x>self->wave->height||y>self->wave->width) PYTHON_ERROR("index out of bound.")
	self->wave->v[y+x*self->wave->width]=value;
	RETURN_NONE;
}
//.......................................................................................
static PyObject* py_wave_setboundarytype(Py_Mechanicwave* self,PyObject* args){
	int type;
	if(!PyArg_ParseTuple(args,"i",&type)) return NULL;
	if(type!=0&&type!=1) PYTHON_ERROR("Invalid type number.\nuse 0 for reflecting boundary and 1 for absorbing boundary.")
	mechanic_wave_set_boundarytype(self->wave,type);
	RETURN_NONE;
}
//.......................................................................................
static PyObject* py_wave_get_maxvalue(Py_Mechanicwave* self,PyObject* args){
	float result=mechanic_wave_get_maxvalue(self->wave);
	return Py_BuildValue("f",result);
}
static PyObject* py_wave_get_maxvelocity(Py_Mechanicwave* self,PyObject* args){
	float result=mechanic_wave_get_maxvelocity(self->wave);
	return Py_BuildValue("f",result);
}
//.......................................................................................
static PyMethodDef py_wave_methods[]={
	{"reset",          (PyCFunction)py_wave_reset,          METH_NOARGS,"reset the mechanic wave."              },
	{"addsource",      (PyCFunction)py_wave_addsource,      METH_VARARGS,"add wave sources."                    },
	{"removesource",   (PyCFunction)py_wave_removesources,  METH_NOARGS, "remove all the sources from the wave."},
	{"getsource",      (PyCFunction)py_wave_getsource,      METH_VARARGS,"get source from the main wave."       },
	{"setv",           (PyCFunction)py_wave_setv,           METH_VARARGS,"set wave velocity."                   },
	{"setboundarytype",(PyCFunction)py_wave_setboundarytype,METH_VARARGS,"set the boundary type of a wave."     },
	{"getmaxvalue",    (PyCFunction)py_wave_get_maxvalue,   METH_NOARGS, "get the maxium amplitude of a wave."  },
	{"getmaxvelocity", (PyCFunction)py_wave_get_maxvelocity,METH_NOARGS, "get maxium velocity of a wave."       },
	{NULL,NULL,0,NULL}
};
//...................define of mechanic wave...............................................
static PyTypeObject py_mechanicwave_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"wave.mechanicwave",              /* tp_name */
	sizeof(Py_Mechanicwave),          /* tp_basicsize */
	0,                                /* tp_itemsize */
	(destructor)py_mechanicwave_free, /* tp_dealloc */
	0,                              /* tp_print */
	0,                              /* tp_getattr */
	0,                              /* tp_setattr */
	0,                              /* tp_reserved */
	(reprfunc)py_mechanicwave_repr, /* tp_repr */
	0,                              /* tp_as_number */
	0,                              /* tp_as_sequence */
	0,                              /* tp_as_mapping */
	0,                              /* tp_hash  */
	0,                              /* tp_call */
	0,                              /* tp_str */
	0,                              /* tp_getattro */
	0,                              /* tp_setattro */
	0,                              /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"mechanic wave object",         /* tp_doc */
	0,                              /* tp_traverse */
	0,                              /* tp_clear */
	0,                              /* tp_richcompare */
	0,                              /* tp_weaklistoffset */
	0,                              /* tp_iter */
	0,                              /* tp_iternext */
	py_wave_methods,                /* tp_methods */
	0,                              /* tp_members */
	0,                              /* tp_getset */
	0,                              /* tp_base */
	0,                              /* tp_dict */
	0,                              /* tp_descr_get */
	0,                              /* tp_descr_set */
	0,                              /* tp_dictoffset */
	(initproc)py_mechanicwave_init, /* tp_init */
	0,                              /* tp_alloc */
      PyType_GenericNew               /* tp_new */
};
//.......................................................................................................................
// python methods of wave source
static int py_wavesource_init(Py_Wavesource* self,PyObject* args){
	//x,y,amplitude,frequency,phase
	float x,y,amplitude,frequency,phase;
	if(!PyArg_ParseTuple(args,"fffff",&x,&y,&amplitude,&frequency,&phase))return -1;
	self->source=mechanic_wave_source_new(x,y,amplitude,frequency,phase);
	return 0;
}
//.........................................................................................
static void py_wavesource_free(Py_Wavesource* self){
	//we can't simply free a source,or source adding won't make sence.
	for(int i=0;i<SEAL(this_wave)->sourcecount;i++){
		if(SEAL(this_wave)->sources[i]==self->source){
			Py_TYPE(self)->tp_free((PyObject *)self);
			return;
		}
	}
	free(self->source);
	Py_TYPE(self)->tp_free((PyObject *)self);
}
//.........................................................................................
static PyObject* py_wavesource_repr(Py_Wavesource* self){
	return Py_BuildValue("s","a wave source.");
}
//.........................................................................................
static PyObject* py_wavesource_setx(Py_Wavesource* self,PyObject* args){
	float x;
	if(!PyArg_ParseTuple(args,"f",&x)) return NULL;
	if(x>400||x<0) PYTHON_ERROR("error:x coordinate out of bound.(x [0,400])")
	self->source->x=x;
	RETURN_NONE;
}
//.........................................................................................
static PyObject* py_wavesource_sety(Py_Wavesource* self,PyObject* args){
	float y;
	if(!PyArg_ParseTuple(args,"f",&y)) return NULL;
	if(y>400||y<0) PYTHON_ERROR("error:y coordinate out oof bound.(y [0,400])")
	self->source->y=y;
	RETURN_NONE;
}
//.........................................................................................
static PyObject* py_wavesource_getx(Py_Wavesource* self,PyObject* args){
	int x=self->source->x;
	return Py_BuildValue("i",x);
}
//.........................................................................................
static PyObject* py_wavesource_gety(Py_Wavesource* self,PyObject* args){
	int y=self->source->y;
	return Py_BuildValue("i",y);
}
//.........................................................................................
static PyObject* py_wavesource_setstate(Py_Wavesource* self,PyObject* args){
	int state;
	if(!PyArg_ParseTuple(args,"i",&state)) return NULL;
	if(state!=1||state!=0) PYTHON_ERROR("Invalid state.\n0 for off,1 for on.")
	self->source->state=state;
	RETURN_NONE;
}
//.........................................................................................
static PyObject* py_wavesource_setvx(Py_Wavesource* self,PyObject* args){
	float vx;
	if(!PyArg_ParseTuple(args,"f",&vx)) return NULL;
	self->source->vx=vx;
	RETURN_NONE;
}
//..........................................................................................
static PyObject* py_wavesource_setvy(Py_Wavesource* self,PyObject* args){
	float vy;
	if(!PyArg_ParseTuple(args,"f",&vy)) return NULL;
	self->source->vy=vy;
	RETURN_NONE;
}
//..........................................................................................
static PyObject* py_wavesource_setamplitude(Py_Wavesource* self,PyObject* args){
	float amp;
	if(!PyArg_ParseTuple(args,"f",&amp)) return NULL;
	self->source->amplitude=amp;
	RETURN_NONE;
}
//..........................................................................................
static PyObject* py_wavesource_setfrequency(Py_Wavesource* self,PyObject* args){
	float f;
	if(!PyArg_ParseTuple(args,"f",&f)) return NULL;
	self->source->frequency=f;
	RETURN_NONE;
}
//..........................................................................................
static PyObject* py_wavesource_setphase(Py_Wavesource* self,PyObject* args){
	float p;
	if(!PyArg_ParseTuple(args,"f",&p)) return NULL;
	self->source->phase=p;
	RETURN_NONE;
}
//.................methods of wave source...........................................................
static PyMethodDef py_wavesource_methods[]={
	{"setX",        (PyCFunction)py_wavesource_setx,        METH_VARARGS,"set x coordinate of the source." },
	{"setY",        (PyCFunction)py_wavesource_sety,        METH_VARARGS,"set x coordinate of the source." },
	{"getX",        (PyCFunction)py_wavesource_getx,        METH_NOARGS, "get x coordinate."               },
	{"getY",        (PyCFunction)py_wavesource_gety,        METH_NOARGS, "get y coordinate."               },
	{"setstate",    (PyCFunction)py_wavesource_setstate,    METH_VARARGS,"set state of a source."          },
	{"setvx",       (PyCFunction)py_wavesource_setvx,       METH_VARARGS,"set x component of a wave source"},
	{"setvy",       (PyCFunction)py_wavesource_setvy,       METH_VARARGS,"set y component of a wave source"},
	{"setamplitude",(PyCFunction)py_wavesource_setamplitude,METH_VARARGS,"set amplitude."                  },
	{"setfrequency",(PyCFunction)py_wavesource_setfrequency,METH_VARARGS,"set frequency."                  },
	{"setphase",    (PyCFunction)py_wavesource_setphase,    METH_VARARGS,"set phase."                      },
	{NULL, NULL, 0, NULL}
};
//......................definations of source...............................................
static PyTypeObject py_wavesource_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"wave.wavesource",              /* tp_name */
	sizeof(Py_Wavesource),          /* tp_basicsize */
	0,                              /* tp_itemsize */
	(destructor)py_wavesource_free, /* tp_dealloc */
	0,                              /* tp_print */
	0,                              /* tp_getattr */
	0,                              /* tp_setattr */
	0,                              /* tp_reserved */
	(reprfunc)py_wavesource_repr,   /* tp_repr */
	0,                              /* tp_as_number */
	0,                              /* tp_as_sequence */
	0,                              /* tp_as_mapping */
	0,                              /* tp_hash  */
	0,                              /* tp_call */
	0,                              /* tp_str */
	0,                              /* tp_getattro */
	0,                              /* tp_setattro */
	0,                              /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"just a test.",                 /* tp_doc */
	0,                              /* tp_traverse */
	0,                              /* tp_clear */
	0,                              /* tp_richcompare */
	0,                              /* tp_weaklistoffset */
	0,                              /* tp_iter */
	0,                              /* tp_iternext */
	py_wavesource_methods,          /* tp_methods */
	0,                              /* tp_members */
	0,                              /* tp_getset */
	0,                              /* tp_base */
	0,                              /* tp_dict */
	0,                              /* tp_descr_get */
	0,                              /* tp_descr_set */
	0,                              /* tp_dictoffset */
	(initproc)py_wavesource_init,   /* tp_init */
	0,                              /* tp_alloc */
      PyType_GenericNew               /* tp_new */
};

//...................................................................
//.....................................................................................
static PyObject* py_wave_addsource(Py_Mechanicwave* self,PyObject* args){
	Py_Wavesource* source;
	if(!PyArg_ParseTuple(args,"O!",&py_wavesource_type,&source)) return NULL;
	mechanic_wave_add_source(self->wave,source->source);
	RETURN_NONE;
}
//.....................................................................................
static Py_Wavesource* py_wave_getsource(Py_Mechanicwave* self,PyObject* args){
	int index;
	if(!PyArg_ParseTuple(args,"i",&index)) return NULL;
	if(index>=self->wave->sourcecount||index<0) PYTHON_ERROR("error:wave source index out of bound.")
	Py_Wavesource* source=(Py_Wavesource*)PyType_GenericNew(&py_wavesource_type,(PyObject*)self,args);
	source->source=self->wave->sources[index];
	return source;
}
//...........................static methods of mechanic wave.........
static Py_Mechanicwave* py_mechanicwave_getwave(PyObject* self,PyObject* args){
	Py_Mechanicwave* wave=(Py_Mechanicwave*)PyType_GenericNew(&py_mechanicwave_type,self,args);
	wave->wave=SEAL(this_wave);
	return wave;
}
//...............................................................................
static PyObject* py_wave_stderr(PyObject* self,PyObject* args){
	char* s=NULL;
	if (!PyArg_ParseTuple(args,"s", &s)) return NULL;
	mechanic_wave_scripterror=s;
	return Py_BuildValue("i",0);
}
//..............................................................................
static PyObject* py_wave_makedialogue(PyObject* self,PyObject* args){
	char* title,*content;
	if(!PyArg_ParseTuple(args,"ss",&title,&content)) return NULL;
	msgbox(title,content);
	RETURN_NONE;
}
//..............................................................................
static PyMethodDef wave_static[]={
	{"stderr",  py_wave_stderr,                      METH_VARARGS,"deal with script errors."},
	{"getwave", (PyCFunction)py_mechanicwave_getwave,METH_VARARGS,"get system wave."        },
	{"mkdialog",(PyCFunction)py_wave_makedialogue,   METH_VARARGS,"show a dialogue box.    "},
	{NULL,NULL,0,NULL}
};
static PyModuleDef py_wave_module={
	PyModuleDef_HEAD_INIT,
	"wave",
	"mechanic wave lib",
	-1,
	wave_static,NULL,NULL,NULL
};
PyObject* module_wave_new(){
	PyObject* module=PyModule_Create(&py_wave_module);
	if(PyType_Ready(&py_wavesource_type)) return NULL;
	Py_INCREF(&py_wavesource_type);
	PyModule_AddObject(module,"wavesource",(PyObject*)&py_wavesource_type);
	if(PyType_Ready(&py_mechanicwave_type)) return NULL;
	Py_INCREF(&py_mechanicwave_type);
	PyModule_AddObject(module,"mechanicwave",(PyObject*)&py_mechanicwave_type);
	return module;
}
PyMODINIT_FUNC module_wave_init(){
	return module_wave_new();
}
int run_script(MechanicWave* target,char* script,char** errcode){
	SEAL(this_wave)=target;
	PyImport_AppendInittab("wave",module_wave_init);
	Py_Initialize();
	PyObject* module=PyImport_ImportModule("wave");
	PyObject* dict=PyModule_GetDict(module);
	PyRun_SimpleString("import wave\n"
				 "import sys\n"
				 "class StderrCatcher:\n"
				 "	def __init__(self):\n"
				 "		self.data=''\n"
				 "	def write(self,stuff):\n"
				 "		self.data+=stuff\n"
				 "		wave.stderr(self.data)\n"
				 "sys.stderr=StderrCatcher()\n"
				 );
	int success=PyRun_SimpleString(script);
	Py_Finalize();
	if(errcode!=NULL) *errcode=mechanic_wave_scripterror;
	return success;
}
