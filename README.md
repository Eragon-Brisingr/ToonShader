# 插件添加ShaderModel

注意该教程在引擎版本**4.26**时作成

## Hack添加ShaderModel大致流程

* 继承UMaterialExpressionCustomOutput  
    使用虚幻中**UMaterialExpressionCustomOutput**类型可在代码编译时插入**条件编译宏**的特性将自定义的ShaderModelId添加进Shader编译流程  
    插件中重写的GetFunctionName返回的**GetToonShading**在shader对应的宏为**NUM_MATERIAL_OUTPUTS_GETTOONSHADING**

* 进入引擎Shader目录修改Shader文件

* 修改**ShadingCommon.ush**中添加新的ShaderModel宏定义
  * 添加宏**SHADINGMODELID_STYLIZED_SHADOW**，且修改**SHADINGMODELID_NUM**的数量（注意当前最多支持16个）

    > ```c++
    > #define SHADINGMODELID_STYLIZED_SHADOW        12
    > #define SHADINGMODELID_NUM                    13
    > ```

  * 修改GetShadingModelColor函数，新增**SHADINGMODELID_STYLIZED_SHADOW**的调试颜色

* 修改**BasePassCommon.ush**与**DeferredShadingCommon.ush**开启自定义GBuffer的CustomData写入  
  * **BasePassCommon.ush**
    **WRITES_CUSTOMDATA_TO_GBUFFER**宏，支持**SHADINGMODELID_STYLIZED_SHADOW**写入

    > ```c++
    > #define WRITES_CUSTOMDATA_TO_GBUFFER        (USES_GBUFFER && (MATERIAL_SHADINGMODEL_SUBSURFACE || MATERIAL_SHADINGMODEL_PREINTEGRATED_SKIN || MATERIAL_SHADINGMODEL_SUBSURFACE_PROFILE || MATERIAL_SHADINGMODEL_CLEAR_COAT || MATERIAL_SHADINGMODEL_TWOSIDED_FOLIAGE || MATERIAL_SHADINGMODEL_HAIR || MATERIAL_SHADINGMODEL_CLOTH || MATERIAL_SHADINGMODEL_EYE || SHADINGMODELID_STYLIZED_SHADOW))
    > ```

  * **DeferredShadingCommon.ush**
    修改**HasCustomGBufferData**函数，支持**SHADINGMODELID_STYLIZED_SHADOW**写入

    > ```c++
    > bool HasCustomGBufferData(int ShadingModelID)
    > {
    >   return ShadingModelID == SHADINGMODELID_SUBSURFACE
    >     || ShadingModelID == SHADINGMODELID_PREINTEGRATED_SKIN
    >     || ShadingModelID == SHADINGMODELID_CLEAR_COAT
    >     || ShadingModelID == SHADINGMODELID_SUBSURFACE_PROFILE
    >     || ShadingModelID == SHADINGMODELID_TWOSIDED_FOLIAGE
    >     || ShadingModelID == SHADINGMODELID_HAIR
    >     || ShadingModelID == SHADINGMODELID_CLOTH
    >     || ShadingModelID == SHADINGMODELID_EYE
    >     || ShadingModelID == SHADINGMODELID_STYLIZED_SHADOW;
    > }
    > ```

* 修改**BasePassPixelShader.usf**中ShadingModel的初始化  

  > ```c++
  > #if NUM_MATERIAL_OUTPUTS_GETTOONSHADING
  >     uint ShadingModel = SHADINGMODELID_STYLIZED_SHADOW;
  > #else
  >     uint ShadingModel = GetMaterialShadingModel(PixelMaterialInputs);
  > #endif
  > ```

* 修改**ShadingModelsMaterial.ush**将额外的数据写入GBuffer的CustomData

  > ```c++
  > // 支持 MaterialExpressionCustomOutput 形式
  > #if NUM_MATERIAL_OUTPUTS_GETTOONSHADING
  >     else if (ShadingModel == SHADINGMODELID_STYLIZED_SHADOW)
  >     {
  >         GBuffer.CustomData.x = saturate( GetToonShading0(MaterialParameters) );  // SpecularRange
  >         GBuffer.CustomData.y = saturate( GetToonShading1(MaterialParameters) );  // Offset
  >     }
  > // 支持 ShadingModelId 形式
  > #elif SHADINGMODELID_STYLIZED_SHADOW
  >     else if (ShadingModel == SHADINGMODELID_STYLIZED_SHADOW)
  >     {
  >         GBuffer.CustomData.x = saturate( GetMaterialCustomData0(MaterialParameters) );  // SpecularRange
  >         GBuffer.CustomData.y = saturate( GetMaterialCustomData1(MaterialParameters) );  // Offset
  >     }
  > #endif
  > ```

* 修改**ShadingModels.ush**支持添加的ShaderModel的自定义BxDF函数  
  **IntegrateBxDF**函数中添加**SHADINGMODELID_STYLIZED_SHADOW**的case

  > ```c++
  > case SHADINGMODELID_STYLIZED_SHADOW:
  >    return StylizedShadowBxDF(GBuffer, N, V, L, Falloff, NoL, AreaLight, Shadow);
  > ```

* 修改**DeferredLightingCommon.ush**自定义光照  
  修改**GetDynamicLightingSplit**函数，通过GBuffer.ShadingModelID判断SHADINGMODELID_STYLIZED_SHADOW执行自定义光照逻辑

  > ```c++
  > // STYLIZEDSHADOW SHADING 
  > if (GBuffer.ShadingModelID == SHADINGMODELID_STYLIZED_SHADOW)
  > {
  >   float3 Attenuation = 1;
  > 
  >   float offset = GBuffer.CustomData.y;
  >   float TerminatorRange = saturate(GBuffer.Roughness - 0.5);
  >   
  >   offset = offset * 2 - 1;
  >   
  >   BRANCH
  >   if (offset >= 1)
  >   {
  >     Attenuation = 1;
  >   }
  >   else
  >   {
  >     float NoL = (dot(N, L) + 1) / 2;
  >     float NoLOffset = saturate(NoL + offset);
  >     float LightAttenuationOffset = saturate( Shadow.SurfaceShadow + offset);
  >     float ToonSurfaceShadow = smoothstep(0.5 - TerminatorRange, 0.5 + TerminatorRange, LightAttenuationOffset);
  > 
  >     Attenuation = smoothstep(0.5 - TerminatorRange, 0.5 + TerminatorRange, NoLOffset) * ToonSurfaceShadow;
  >   }
  > 
  >   LightAccumulator_AddSplit( LightAccumulator, Lighting.Diffuse, Lighting.Specular, Lighting.Diffuse, LightColor * LightMask * Shadow.SurfaceShadow * Attenuation * 0.25, > bNeedsSeparateSubsurfaceLightAccumulation);
  > }
  > else
  > {
  >   LightAccumulator_AddSplit( LightAccumulator, Lighting.Diffuse, Lighting.Specular, Lighting.Diffuse, LightColor * LightMask * Shadow.SurfaceShadow, bNeedsSeparateSubsurfaceLightAccumulation );
  > }
  > ```

* 在材质编辑器中添加**Toon Shading**节点，ShaderModel即切换为**SHADINGMODELID_STYLIZED_SHADOW**  
  *不再受编辑器中的SaherModel*控制  
  SpecularRange输入0，Offset输入为1，即可看到自定义材质的效果

## 插件支持ShaderModel的方式

* ToonShader模块  
  该模块实现了**UMaterialExpressionCustomOutput**的子类，为运行时模块

* ToonShaderBootstrap模块  
  该模块会在引擎启动Shader编译前加载，用以向虚幻引擎Shader文件夹中替换默认Shader文件  
  ShadersOverride文件夹下存放了shader文件
  * Default文件用来恢复
  * Override用来做替换，新增的Shader文件都需要存放在Toon目录下
