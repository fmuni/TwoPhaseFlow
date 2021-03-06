import casefoam
import math
case = [['gradAlpha','RDF','fitParaboloid'],
        ['15','30','45','60','75'],
        ['Grid1', 'Grid2', 'Grid3', 'Grid4', 'Grid5']]


def updateInitAlpha(angle):
    s = 1
    wallAngle = 180 - angle
    radius = s/math.sin(math.radians(90-angle))/2
    return {
            'system/initAlphaFieldDict':
                { 'radius': '%s' % radius,
                 'origin': '(0 1 0)'},
            '0.orig/alpha1' :  {'boundaryField': {'walls': {'theta0': '%s' % wallAngle}}}}

update_gradAlpha = {
    'constant/transportProperties': {
        'surfaceForces': {'curvatureModel': 'gradAlpha'}}}

update_RDF = {
    'constant/transportProperties': {
        'surfaceForces': {'curvatureModel': 'RDF'}}}

update_fitParaboloid = {
    'constant/transportProperties': {
        'surfaceForces': {'curvatureModel': 'fitParaboloid'}}}

def updateBlockMesh(x):
    return  {'system/blockMeshDict': {
            'blocks': ['hex',
                   [0, 1, 2, 3, 4, 5, 6, 7],
                   '(%s %s 1)' % (x , 2*x),
                   'simpleGrading',
                   '(1 1 1)']}}


data = {'gradAlpha': update_gradAlpha,
        'RDF': update_RDF,
        'fitParaboloid': update_fitParaboloid,
        '15' : updateInitAlpha(15),
        '30' : updateInitAlpha(30),
        '45' : updateInitAlpha(45),
        '60' : updateInitAlpha(60),
        '75' : updateInitAlpha(75),
        'Grid1': updateBlockMesh(32),
        'Grid2': updateBlockMesh(64),
        'Grid3': updateBlockMesh(128),
        'Grid4': updateBlockMesh(256),
        'Grid5': updateBlockMesh(512)}

casefoam.mkCases('curvatureWall', case, data, 'tree',writeDir='Cases')
