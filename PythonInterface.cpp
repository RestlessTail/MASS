#include "PythonInterface.h"
#include <Python.h>
#include "../MAS/CInterface.h"
#include <structmember.h>

static PyObject* _InitEngine(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    InitEngine();
    Py_RETURN_NONE;
}

static PyObject* _LoadTextureResource(PyObject* self, PyObject* args)
{
    char* arg;
    unsigned int res;
    if (!PyArg_ParseTuple(args, "s", &arg))
        return NULL;
    res = LoadTextureResource(arg);
    return PyLong_FromLong(res);
}

static PyObject* _LoadSoundResource(PyObject* self, PyObject* args)
{
    char* arg;
    int res;
    if (!PyArg_ParseTuple(args, "s", &arg))
        return NULL;
    res = LoadSoundResource(arg);
    return PyLong_FromLong(res);
}

static PyObject* _FreeTextureResource(PyObject* self, PyObject* args)
{
    int arg;
    if (!PyArg_ParseTuple(args, "i", &arg))
        return NULL;
    FreeTextureResource(arg);
    Py_RETURN_NONE;
}

static PyObject* _FreeSoundResource(PyObject* self, PyObject* args)
{
    int arg;
    if (!PyArg_ParseTuple(args, "i", &arg))
        return NULL;
    FreeSoundResource(arg);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Finish(PyObject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    PlayAction_Finish();
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Voiceover(PyObject* self, PyObject* args)
{
    Py_UNICODE* buf;
    if (!PyArg_ParseTuple(args, "u", &buf))
        return NULL;

    PlayAction_Voiceover(buf);

    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Say_ByName(PyObject* self, PyObject* args)
{
    Py_UNICODE* n;
    Py_UNICODE* c;
    if (!PyArg_ParseTuple(args, "uu", &n, &c))
        return NULL;

    PlayAction_Say_ByName(n, c);

    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Say_ByActor(PyObject* self, PyObject* args)
{
    int a;
    Py_UNICODE* c;
    if (!PyArg_ParseTuple(args, "iu", &a, &c))
        return NULL;

    PlayAction_Say_ByActor(a, c);

    Py_RETURN_NONE;
}

static PyObject* _PlayAction_UseScene(PyObject* self, PyObject* args)
{
    int arg;
    if (!PyArg_ParseTuple(args, "i", &arg))
        return NULL;
    PlayAction_UseScene(arg);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Enter(PyObject* self, PyObject* args)
{
    int a;
    char* pos;
    if (!PyArg_ParseTuple(args, "is", &a, &pos))
        return NULL;
    PlayAction_Enter(a, *pos);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Exit_ByActor(PyObject* self, PyObject* args)
{
    int a;
    if (!PyArg_ParseTuple(args, "i", &a))
        return NULL;
    PlayAction_Exit_ByActor(a);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Exit_ByPosition(PyObject* self, PyObject* args)
{
    char* pos;
    if (!PyArg_ParseTuple(args, "s", &pos))
        return NULL;
    PlayAction_Exit_ByPosition(*pos);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Attack_ByActor(PyObject* self, PyObject* args)
{
    int a;
    if (!PyArg_ParseTuple(args, "i", &a))
        return NULL;
    PlayAction_Attack_ByActor(a);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Attack_ByPosition(PyObject* self, PyObject* args)
{
    char* pos;
    if (!PyArg_ParseTuple(args, "s", &pos))
        return NULL;
    PlayAction_Attack_ByPosition(*pos);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Delay(PyObject* self, PyObject* args)
{
    int val;
    if (!PyArg_ParseTuple(args, "i", &val))
        return NULL;
    PlayAction_Delay(val);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Retreat_ByActor(PyObject* self, PyObject* args)
{
    int a;
    if (!PyArg_ParseTuple(args, "i", &a))
        return NULL;
    PlayAction_Retreat_ByActor(a);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_Retreat_ByPosition(PyObject* self, PyObject* args)
{
    char* pos;
    if (!PyArg_ParseTuple(args, "s", &pos))
        return NULL;
    PlayAction_Retreat_ByPosition(*pos);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_PlaySound(PyObject* self, PyObject* args)
{
    int s;
    int t;
    int l = 0;
    if (!PyArg_ParseTuple(args, "ii|i", &s, &t, &l))
        return NULL;
    PlayAction_PlaySound(s, t, l);
    Py_RETURN_NONE;
}

static PyObject* _PlayAction_StopSound(PyObject* self, PyObject* args)
{
    int t;
    if (!PyArg_ParseTuple(args, "i", &t))
        return NULL;
    PlayAction_StopSound(t);
    Py_RETURN_NONE;
}

static PyObject* _GenerateActor(PyObject* self, PyObject* args)
{
    Py_UNICODE* n;
    int res;
    if (!PyArg_ParseTuple(args, "u", &n))
        return NULL;
    res = GenerateActor(n);
    return PyLong_FromLong(res);
}

static PyObject* _AddTextureForActor(PyObject* self, PyObject* args)
{
    int a;
    Py_UNICODE* k;
    int t;
    if (!PyArg_ParseTuple(args, "iui", &a, &k, &t))
        return NULL;
    AddTextureForActor(a, k, t);
    Py_RETURN_NONE;
}

static PyObject* _SetActiveTextureForActor(PyObject* self, PyObject* args)
{
    int a;
    Py_UNICODE* k;
    if (!PyArg_ParseTuple(args, "iu", &a, &k))
        return NULL;
    SetActiveTextureForActor(a, k);
    Py_RETURN_NONE;
}

static PyMethodDef MASSMethods[] = {
    {"InitEngine", _InitEngine, METH_VARARGS, ""},
    {"LoadTextureResource", _LoadTextureResource, METH_VARARGS, ""},
    {"LoadSoundResource", _LoadSoundResource, METH_VARARGS, ""},
    {"FreeTextureResource", _FreeTextureResource, METH_VARARGS, ""},
    {"FreeSoundResource", _FreeSoundResource, METH_VARARGS, ""},
    {"PlayAction_Finish", _PlayAction_Finish, METH_VARARGS, ""},
    {"PlayAction_Voiceover", _PlayAction_Voiceover, METH_VARARGS, ""},
    {"PlayAction_Say_ByName", _PlayAction_Say_ByName, METH_VARARGS, ""},
    {"PlayAction_Say_ByActor", _PlayAction_Say_ByActor, METH_VARARGS, ""},
    {"PlayAction_UseScene", _PlayAction_UseScene, METH_VARARGS, ""},
    {"PlayAction_Enter", _PlayAction_Enter, METH_VARARGS, ""},
    {"PlayAction_Exit_ByActor", _PlayAction_Exit_ByActor, METH_VARARGS, ""},
    {"PlayAction_Exit_ByPosition", _PlayAction_Exit_ByPosition, METH_VARARGS, ""},
    {"PlayAction_Attack_ByActor", _PlayAction_Attack_ByActor, METH_VARARGS, ""},
    {"PlayAction_Attack_ByPosition", _PlayAction_Attack_ByPosition, METH_VARARGS, ""},
    {"PlayAction_Delay", _PlayAction_Delay, METH_VARARGS, ""},
    {"PlayAction_Retreat_ByActor", _PlayAction_Retreat_ByActor, METH_VARARGS, ""},
    {"PlayAction_Retreat_ByPosition", _PlayAction_Retreat_ByPosition, METH_VARARGS, ""},
    {"PlayAction_PlaySound", _PlayAction_PlaySound, METH_VARARGS, ""},
    {"PlayAction_StopSound", _PlayAction_StopSound, METH_VARARGS, ""},
    {"GenerateActor", _GenerateActor, METH_VARARGS, ""},
    {"AddTextureForActor", _AddTextureForActor, METH_VARARGS, ""},
    {"SetActiveTextureForActor", _SetActiveTextureForActor, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef MASS =
{
    PyModuleDef_HEAD_INIT,
    "MASS", /* name of module */
    "https://git\n", /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    MASSMethods
};

PyMODINIT_FUNC PyInit_MASS(void)
{
    return PyModule_Create(&MASS);
}