/*---------------------------------------------------------------------------*\
            Copyright (c) 2017-2019, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the VoFLibrary source code library, which is an
	unofficial extension to OpenFOAM.
    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.
    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "gravity.H"
#include "gravityMeshObject.H"
#include "addToRunTimeSelectionTable.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(gravity, 0);
    addToRunTimeSelectionTable(accelerationModel,gravity, components);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::gravity::gravity
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    accelerationModel
    (
        typeName,
        dict,
        mesh
    ),
    gravityDict_(dict),
    g_
    (
        "gravity",
        dimAcceleration,
        vector(0,0,0)
    ),
    hRef_
    (
        IOobject
        (
            "hRef",
            mesh.time().constant(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        dimensionedScalar(dimLength, Zero)
    )


{

    calculateAcc();
}


// * * * * * * * * * * * * * * Public Access Member Functions  * * * * * * * //

void Foam::gravity::calculateAcc()
{
    // read only if mesh changed would be clever
    //g_ =
    const fvMesh& mesh = acc_.mesh();
    const uniformDimensionedVectorField& g = meshObjects::gravity::New(mesh.time());
    g_.value() = g.value();

    dimensionedScalar ghRef
    (
        mag(g_.value()) > SMALL
      ? g_ & (cmptMag(g_.value())/mag(g_.value()))*hRef_
      : dimensionedScalar("ghRef", g_.dimensions()*dimLength, 0)
    );

    //Info<< "ghRef dim" << ghRef.dimensions() << endl;

    acc_ = (g_ & mesh.C()) - ghRef;
    accf_ = (g_ & mesh.Cf()) - ghRef;
}


// ************************************************************************* //
