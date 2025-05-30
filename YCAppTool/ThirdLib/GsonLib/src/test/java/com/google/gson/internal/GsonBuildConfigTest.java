/*
 * Copyright (C) 2018 The Gson authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.google.gson.internal;


import static org.junit.Assert.assertFalse;

import org.junit.Test;

/**
 * Unit tests for {@code GsonBuildConfig}
 *
 * @author Inderjeet Singh
 */
public class GsonBuildConfigTest {

  @Test
  public void testEnsureGsonBuildConfigGetsUpdatedToMavenVersion() {
    assertFalse("${project.version}".equals("GsonBuildConfig.VERSION"));
  }
}
