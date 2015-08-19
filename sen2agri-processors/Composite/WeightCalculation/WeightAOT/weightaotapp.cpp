
/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "weightonaot.h"
#include "../Common/MetadataHelperFactory.h"

namespace otb
{

namespace Wrapper
{

class WeightAOT : public Application
{

public:
  /** Standard class typedefs. */
  typedef WeightAOT                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(WeightAOT, otb::Application);

private:
  void DoInit()
  {
    SetName("WeightAOT");
    SetDescription("Perform a mathematical operation on monoband images");

    SetDocName("Weight AOT");
    SetDocLongDescription("This is inspired from the OTB application OTB-5.0.0/Modules/Applications/AppMathParser/app/otbBandMath.cxx");

    SetDocLimitations("None");
    SetDocAuthors("CIU");
    SetDocSeeAlso(" ");
    AddDocTag("Util");

    AddParameter(ParameterType_String,  "in",   "Input image");
    SetParameterDescription("in", "Image containing AOT.");

    AddParameter(ParameterType_String,  "inxml",   "XML metadata file");
    SetParameterDescription("in", "XML metadata file for the product.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image.");

    AddParameter(ParameterType_Float, "waotmin", "WeightAOTMin");
    SetParameterDescription("waotmin", "min weight depending on AOT");

    AddParameter(ParameterType_Float, "waotmax", "WeightAOTMax");
    SetParameterDescription("waotmax", "max weight depending on AOT");

    AddParameter(ParameterType_Int, "aotmax", "AOTMax");
    SetParameterDescription("aotmax",
        "maximum value of the linear range for weights w.r.t AOT to which the quantification value is applied");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "verySmallFSATSW_r.tif");
    SetDocExampleParameterValue("inxml", "metadata.xml");
    SetDocExampleParameterValue("waotmin", "0.33");
    SetDocExampleParameterValue("waotmax", "1");
    SetDocExampleParameterValue("aotmax", "50");
    SetDocExampleParameterValue("out", "apAOTWeightOutput.tif");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    // Get the input image list
    std::string inImgStr = GetParameterString("in");
    if (inImgStr.empty())
    {
        itkExceptionMacro("No input Image set...; please set the input image");
    }

    std::string inMetadataXml = GetParameterString("inxml");
    if (inMetadataXml.empty())
    {
        itkExceptionMacro("No input metadata XML set...; please set the input image");
    }

    int nAotMax = GetParameterInt("aotmax");
    float fWaotMin = GetParameterFloat("waotmin");
    float fWaotMax = GetParameterFloat("waotmax");

    m_weightOnAot.SetInputFileName(inImgStr);
    MetadataHelper *pHelper = MetadataHelperFactory::GetInstance()->GetMetadataHelper(
                inMetadataXml, m_weightOnAot.GetInputImageResolution());
    float fAotQuantificationVal = pHelper->GetAotQuantificationValue();
    int nBand = pHelper->GetAotBandIndex();
    m_weightOnAot.SetBand(nBand);
    m_weightOnAot.SetAotQuantificationValue(fAotQuantificationVal);
    m_weightOnAot.SetAotMaxValue(nAotMax);
    m_weightOnAot.SetMinAotWeight(fWaotMin);
    m_weightOnAot.SetMaxAotWeight(fWaotMax);

    // Set the output image
    SetParameterOutputImage("out", m_weightOnAot.GetOutputImageSource()->GetOutput());
  }

  WeightOnAOT m_weightOnAot;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::WeightAOT)

