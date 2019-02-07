/************************************************************************
 *
 * Copyright (C) 2017-2019 IRCAD France
 * Copyright (C) 2017-2019 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#pragma once

#include "fwRenderOgre/vr/RayTracingVolumeRenderer.hpp"

#include <OGRE/OgreCompositorInstance.h>

namespace fwRenderOgre
{
namespace vr
{

/**
 * @brief Importance driven volume renderer, extends the standard ray tracing renderer.
 */
class FWRENDEROGRE_CLASS_API ImportanceDrivenVolumeRenderer : public RayTracingVolumeRenderer
{
public:

    enum class IDVRCSGModulationMethod
    {
        COLOR1,
        COLOR2,
        COLOR3
    };

    enum class IDVRCSGGrayScaleMethod
    {
        AVERAGE_GRAYSCALE,
        LIGHTNESS_GRAYSCALE,
        LUMINOSITY_GRAYSCALE
    };

    FWRENDEROGRE_API ImportanceDrivenVolumeRenderer(std::string _parentId,
                                                    Layer::sptr _layer,
                                                    ::Ogre::SceneNode* _parentNode,
                                                    ::Ogre::TexturePtr _imageTexture,
                                                    ::Ogre::TexturePtr _maskTexture,
                                                    const TransferFunction::sptr& _gpuVolumeTF,
                                                    const TransferFunction::sptr& _gpuCSGTF,
                                                    PreIntegrationTable& _preintegrationTable,
                                                    bool _ambientOcclusion,
                                                    bool _colorBleeding,
                                                    bool _shadows               = false,
                                                    double _aoFactor            = 1.,
                                                    double _colorBleedingFactor = 1.);

    FWRENDEROGRE_API virtual ~ImportanceDrivenVolumeRenderer();

    /// Called when the transfer function is updated.
    FWRENDEROGRE_API virtual void updateCSGTF();

    /// Set the importance driven method used during the rendering.
    FWRENDEROGRE_API void setIDVRMethod(std::string method);

    /// Toggle countersink geometry when using Importance Driven Volume Rendering.
    FWRENDEROGRE_API void toggleIDVRCountersinkGeometry(bool);

    /// Sets pre-integrated mode.
    FWRENDEROGRE_API virtual void setPreIntegratedRendering(bool) override;

    /// Toggle TF use for countersink geometry .
    FWRENDEROGRE_API void toggleIDVRCSGTF(bool);

    /// Toggle lighting use for countersink geometry .
    FWRENDEROGRE_API void toggleIDVRCSGLighting(bool);

    /// Set the countersink's geometry angle (in degrees).
    FWRENDEROGRE_API void setIDVRCountersinkAngle(double);

    /// Set the countersink geometry blur weight factor used in the MImP method.
    FWRENDEROGRE_API void setIDVRCSGBlurWeight(double);

    /// Toggle context discard when using MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGDisableContext(bool);

    /// Set the countersink geometry depth lines color used in the MImP method.
    FWRENDEROGRE_API void setIDVRCSGDepthLinesColor(std::array<std::uint8_t, 4>);

    /// Set the countersink geometry depth lines width used in the MImP method.
    FWRENDEROGRE_API void setIDVRCSGDepthLinesWidth(float);

    /// Toggle the modulation for MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGModulation(bool);

    /// Set the modulation method used for MImP countersink geometry.
    FWRENDEROGRE_API void setIDVRCSGModulationMethod(IDVRCSGModulationMethod);

    /// Set the weighting factor for MImP CSG color modulation.
    FWRENDEROGRE_API void setIDVRCSGModulationFactor(double);

    /// Toggle the grayscale for MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGGrayScale(bool);

    /// Set the grayscale method used for MImP countersink geometry.
    FWRENDEROGRE_API void setIDVRCSGGrayScaleMethod(IDVRCSGGrayScaleMethod);

    /// Toggle the opacity decrease for MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGOpacityDecrease(bool);

    /// Set the opacity decrease factor used in the MImP CSG.
    FWRENDEROGRE_API void setIDVRCSGOpacityDecreaseFactor(double);

    /// Toggle the depth lines for MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGDepthLines(bool);

    /// Set the number of image spacing units between depth lines.
    FWRENDEROGRE_API void setIDVRCSGDepthLinesSpacing(int);

    /// Toggle the border lines for MImP countersink geometry.
    FWRENDEROGRE_API void toggleIDVRCSGBorder(bool);

    /// Set the countersink geometry border color used in the MImP method.
    FWRENDEROGRE_API void setIDVRCSGBorderColor(std::array<std::uint8_t, 4>);

    /// Set the countersink geometry border width used in the MImP method.
    FWRENDEROGRE_API void setIDVRCSGBorderWidth(float);

    /// Set the alpha correction factor used in the VPImC method.
    FWRENDEROGRE_API void setIDVRAImCAlphaCorrection(double);

    /// Set the alpha correction factor used in the VPImC method.
    FWRENDEROGRE_API void setIDVRVPImCAlphaCorrection(double);

    /// Set the visualized image's spacing.
    FWRENDEROGRE_API void setImageSpacing(const ::Ogre::Vector3& _spacing);

    /// Slot: Called when the size of the viewport changes.
    FWRENDEROGRE_API virtual void resizeViewport(int _w, int _h) override;

protected:

    /// Update the current compositor name according to VR effects flags.
    /// @return tuple containing a
    /// - Comma separated list of preprocessor defines to use in vertex shaders.
    /// - Comma separated list of preprocessor defines to use in fragment shaders.
    /// - Hash allowing to identify the material
    FWRENDEROGRE_API virtual std::tuple<std::string, std::string, size_t> computeRayTracingDefines() const override;

    /// Set all texture units needed by the material during the ray casting pass.
    FWRENDEROGRE_API virtual void setRayCastingPassTextureUnits(::Ogre::Pass* const _rayCastingPass,
                                                                const std::string& _fpPPDefines) const override;

private:

    /// Add the IDVR technique to the volume ray tracing material.
    void createIDVRTechnique();

    /// Generate the material with the IDVR technique.
    void createMaterialAndIDVRTechnique();

    /// Initialize the compositors used after the step computing the ray entry points
    void initCompositors();

    /// Create and adds importance compositing compositors to the chain (MImP + JFA, AImC or VPImC).
    void buildICCompositors(::Ogre::Viewport* const _vp);

    /// Remove all listeners and compositors from the current chain.
    void cleanCompositorChain(::Ogre::Viewport* const _vp);

    /// Texture of the segmentation mask.
    ::Ogre::TexturePtr m_maskTexture;

    /// Name of the method used for Importance driven volume rendering
    std::string m_idvrMethod;

    /// Sets usage of countersink geometry for MImP.
    bool m_idvrCSG {true};

    /// Sets usage of countersink geometry TF.
    bool m_idvrCSGTF {false};

    /// Sets usage of countersink geometry lighting.
    bool m_idvrCSGLighting {false};

    /// Sets the csg's angle cosine for MImP.
    float m_idvrCSGAngleCosine;

    /// Sets countersink geometry blur weight for MImP.
    float m_idvrCSGBlurWeight {0.01f};

    /// Sets usage of countersink geometry border for MImP.
    bool m_idvrCSGBorder {false};

    /// Sets whether or not the context should be discarded when using MImP countersink geometry.
    bool m_idvrCSGDisableContext {false};

    /// Sets usage of modulation for MImP CSG.
    bool m_idvrCSGModulation {false};

    /// Name of the method used to compute the new color values in CSG.
    IDVRCSGModulationMethod m_idvrCSGModulationMethod {IDVRCSGModulationMethod::COLOR1};

    /// Sets usage of grayscale for MImP CSG.
    bool m_idvrCSGGrayScale {false};

    /// Name of the method used to compute the new color values in CSG.
    IDVRCSGGrayScaleMethod m_idvrCSGgrayscaleMethod {IDVRCSGGrayScaleMethod::AVERAGE_GRAYSCALE};

    /// Sets usage of opacity decrease for MImP CSG.
    bool m_idvrCSGOpacityDecrease {false};

    /// Sets usage of depth lines for MImP CSG.
    bool m_idvrCSGDepthLines {false};

    RayEntryCompositor m_idvrMaskRayEntriesCompositor;

    /// Compositor listener classes used to upload uniforms for mono/stereo ray tracing materials.
    class JFACompositorListener;

    /// List of all listeners associated to the VR's compositor chain.
    /// If a compositor has no listener, we store a nullptr in the corresponding entry.
    std::vector< ::Ogre::CompositorInstance::Listener*> m_compositorListeners;

    /// TF texture used for IDVR rendering.
    TransferFunction::wptr m_gpuCSGTF;

    /// Shared parameters used for MImP CSG.
    ::Ogre::GpuSharedParametersPtr m_CSGSharedParameters;

};

} // namespace vr
} // namespace fwRenderOgre